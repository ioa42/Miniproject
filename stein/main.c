#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "labyrinth.h"
#include <math.h>

Graph *parse(char *filename) {

    FILE*   fp;
    char*   line = NULL;
    size_t  len = 0, read;

    fp = fopen(filename, "r");
    if (!fp) { exit(EXIT_FAILURE); }
    Graph *g = NULL;

    while ((read = getline(&line, &len, fp)) != (size_t)-1) {
        char *pch = strtok (line," ");
        if(strcmp(pch, "ROOMS:") == 0) {
            int order = atoi(strtok(NULL, " "));
            g = make_graph(order);
        } else if(strcmp(pch, "MONSTER:") == 0) {
            int src  = atoi(strtok(NULL, " "));
            int cost = atoi(strtok(NULL, " "));
            set_cost(g, src, -cost);
        } else if(strcmp(pch, "BREWERY:") == 0) {
            int src  = atoi(strtok(NULL, " "));
            int cost = atoi(strtok(NULL, " "));
            set_cost(g, src, cost);
        } else if(strcmp(pch, "DEAD:") == 0) {
            int src  = atoi(strtok(NULL, " "));
            set_death(g, src);
        } else if(strcmp(pch, "TUNNEL:") == 0) {
            int src  = atoi(strtok(NULL, " "));
            int cost = atoi(strtok(NULL, " "));
            int dst  = atoi(strtok(NULL, " "));
            add_tunnel(g, src, dst, cost); 
        }
        pch = strtok (NULL, " ");

    }
    free(line);
    fclose(fp);
    return g;
}

void parse_command(char *cmd, Graph *g) {
    char *pch = strtok (cmd," ");
    if(strcmp(pch, "COST") == 0) {
        int life = atoi(strtok(NULL, " "));
        while(*cmd != '[') { ++cmd; }
        ++cmd;
        cost(g, cmd, life);
    } else if(strcmp(pch, "PATH") == 0) {
        int life = atoi(strtok(NULL, " "));
        int src  = atoi(strtok(NULL, " "));
        int dst  = atoi(strtok(NULL, " "));
        
        path(g, life, src, dst);
    }
}

int main(int argc, char **argv) {
    if(argc != 2) { printf("Not enough args"); }
    Graph *g = parse(argv[1]);

    char *line = NULL;
    size_t size;
    while (getline(&line, &size, stdin) != -1) {
        parse_command(line, g);
    }
    free_Graph(g);
    return 0;
}

