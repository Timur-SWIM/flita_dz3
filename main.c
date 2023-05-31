#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NODES 100

typedef struct graph {
    int nodes[MAX_NODES][MAX_NODES];
    int countNode;
}graph;

void dfs(graph *g, int v, int visited[]){
    visited[v] = 1;
    for (int i = 0; i < g->countNode; i++){
        if (g->nodes[v][i] && !visited[i]){
            dfs(g, i, visited);
        }
    }
}

int isConnected(graph *g){
    int visited[MAX_NODES] = {0};
    dfs(g, 0, visited);
    for (int i = 0; i < g->countNode; i++){
        if (!visited[i])
            return 0;
    }
    return 1;
}

void graphFromFile(FILE *inputf, graph *g){
    char line[128];
    int row = 0, col = 0;

    while (fgets(line, sizeof(line), inputf)){
        char *token = strtok(line, " ");
        while (token){
            int value = atoi(token);
            g->nodes[row][col++] = value;
            if (col - 1 <= row)
                g->nodes[col - 1][row] = value;
            token = strtok(NULL, " ");
        }
        row++;
    }
    g->countNode = row;
}

void writeDOT(FILE *outf, graph *g){
    fprintf(outf, "digraph G {\n");
    for (int i = 0; i < g->countNode; i++) {
        fprintf(outf, "  %d;\n", i);
    }
    for (int i = 0; i < g->countNode; i++) {
        for (int j = i; j < g->countNode; j++) {
            if (g->nodes[i][j]) {
                fprintf(outf, "  %d -> %d [dir=none, label=%d];\n", i, j, g->nodes[i][j]);
            }
        }
    }
    fprintf(outf, "}\n");
}

int main(int argc, char *argv[]){
    graph *g = malloc(sizeof(graph));
    FILE *inputf = fopen(argv[1], "rt");
    if (inputf == NULL){
        puts("ERROR: Unable to open file");
        exit(EXIT_FAILURE);
    }
    graphFromFile(inputf, g);
    fclose(inputf);

    if (!isConnected(g)){
        puts("Graph is not connected");
        return 1;
    }
    FILE *outputf = fopen("graph.dot", "w");
    if (outputf == NULL){
        puts("ERROR: Unable to open file");
        return 1;
    }
    writeDOT(outputf, g);
    fclose(outputf);
    free(g);
    system("dot -Tpng graph.dot -o graph.png");
    return 0;
}