#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "labyrinth.h"

Graph *make_graph(int order) {
    Graph *g = malloc(sizeof(Graph));
    g->order = order;
    g->rooms  = malloc(sizeof(Room) * order);
    for(int i = 1; i < order; ++i) {
       room(g, i)->id = i;
       room(g, i)->cost = 0;
       room(g, i)->death = 0;
       room(g, i)->exits = 0;
       room(g, i)->outGoing = NULL;
    }
    return g;
}

void add_tunnel(Graph *g, int src, int dst, int cost) {
    Path *next = room(g, src)->outGoing;
    Path *curr = malloc(sizeof(Path));

    curr->cost = cost;
    curr->dst  = dst;
    curr->next = next;

    room(g, src)->outGoing = curr;
    ++room(g, src)->exits;
}

void set_cost(Graph *g, int src, int cost) {
    room(g, src)->cost = cost;
}

void set_death(Graph *g, int src) {
    room(g, src)->death = 1;
}

Path *get_Path(Graph *g, int src, int dst) {
    Path *p = room(g, src)->outGoing;
    while(p && p->dst != dst) {
        p = p->next;
    }
    return p;
}

int is_dead(Graph *g, int life, int room) {
    if(life <= 0)            { printf("DEAD\n"); return 1; }
    if(room(g, room)->death) { printf("DEAD\n"); return 1; }
    return 0;
}

void cost(Graph *g, char *rooms, int life) { 
    char *buff = strtok(rooms, ";");
    
    // checking src
    int src = atoi(buff), dst;
    life += room(g, src)->cost;
    if(is_dead(g, life, src)) { return; } 
    
    buff = strtok(NULL, ";");
    int roomsNum = 1;
    do {
        // get data
        dst  = atoi(buff);
        Path *p = get_Path(g, src, dst);
        if(p == NULL) { printf("NP\n"); return; }
        
        life -= p->cost;
        life += room(g, dst)->cost;

        if(is_dead(g, life, dst)) { return; }
        
        src = dst; ++roomsNum;
    } while((buff = strtok(NULL, ";]")));
    printf("(%d, %d)\n", life, roomsNum);
}


void add_list(State **list, int life, int room, int cost, char *p) {
    State *s = malloc(sizeof(State));
    s->life  = life;
    s->room  = room;
    s->cost  = cost;
    s->print = p;
    if(!*list || (*list)->cost > cost) {
        s->nxt = *list;
        *list = s;
        return;
    }

    State *curr = *list;
    while(curr->nxt && curr->nxt->cost < cost) {
        curr = curr->nxt;
    }

    s->nxt = curr->nxt;
    curr->nxt = s;
}

State *pop_min(State **list) {
    State *s = *list;
    *list = (*list)->nxt;
    return s;
}

int sizeofint(int n) {
    int s = 0;
    while(n != 0) {
        n /= 10;
        ++s;
    }
    return s;
}

void get_states(Graph *g, State **list, int src, int life, int cost, char *path) {
   Path *p = room(g, src)->outGoing;
    int sz = strlen(path);
   for(int i = 0; i < room(g, src)->exits; ++i) {
        if(!room(g, p->dst)->death) {
            int nlife = life - p->cost + room(g, p->dst)->cost;
            if(nlife > 0) {
                int size = sz + 2 + sizeofint(p->dst);
                char *npath = malloc(size);
                snprintf(npath, size, "%s;%d", path, p->dst);
                add_list(list, nlife, p->dst, cost + 1, npath);
            }
        }
        p = p->next;
   }
}



void path(Graph *g, int life, int src, int dst) {
    State **list = malloc(sizeof(State*));
    (*list) = NULL;
    int sz = sizeofint(src) + 2;
    char *buff = malloc(sz); // [ & \0
    snprintf(buff, sz, "[%d", src);
    add_list(list, life, src, 0, buff);

    while(*list) {
        State *s = pop_min(list);
        //printf("(%d, %d)\n", s->room, s->life);
        if(s->room == dst) {
            while(*list) {
                State *freeing = pop_min(list);
                free(freeing->print);
                free(freeing);
            }
            free(list);
            printf("%s]\n", s->print);
            free(s->print);
            free(s);
            return;
        }
        get_states(g, list, s->room, s->life, s->cost, s->print);
        free(s->print);
        free(s);
    }

    free(list);
}

void free_Path(Graph *g, int r) {
    Path *p = room(g, r)->outGoing;
    Path *nxt = NULL;
    while(p) {
        nxt = p->next;
        free(p);
        p = nxt;
    }
}

void free_Graph(Graph *g) {
    for(int i = 1; i < g->order; ++i) {
        free_Path(g, i);
    }
    free(g->rooms);
    free(g);
}
