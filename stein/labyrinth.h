#ifndef __LABYRINTH__
#define __LABYRINTH__


#define room(g,s) (g->rooms + s - 1)

typedef struct s_Path  Path;
typedef struct s_Room  Room;
typedef struct s_State State;

struct s_Path {
    int cost;
    int dst;
    Path *next;
};

struct s_Room {
    int id;
    int cost;
    int death;
    int exits;
    Path *outGoing;
};

typedef struct {
    int order;
    Room *rooms;
} Graph;

struct s_State {
    int life;
    int room;
    int cost;
    char *print;
    State *nxt;
};

Graph *make_graph(int order);
void add_tunnel(Graph *g, int src, int dst, int cost);
void set_cost(Graph *g, int src, int dst);
void set_death(Graph *g, int src);

// @return the path between src & dst.
// Will be NULL is it doesn't exist
Path *get_Path(Graph *g, int src, int dst);

void cost(Graph *g, char *rooms, int life);
void path(Graph *g, int life, int src, int dst);

void free_Graph(Graph *g);

#endif
