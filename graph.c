#include <stdlib.h>
//#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"

/* This is a testing file just to be able to compile the 
 * examples of binary files to convert graph from txt->bin
 * and bin -> txt
 */

// - function -----------------------------------------------------------------
graph_t* allocate_graph(void)//added int numnodes
{
    graph_t* g = (graph_t*)malloc(sizeof(graph_t));
    if (g == NULL) {
        return NULL;
      }
    g->num_edges = 0;
    g->capacity = 10;
    g->edges = calloc(g->capacity, sizeof(edge_t));
    if (g->edges == NULL) {
        free(g);
        return NULL;
    }


   return g;
}

// - function -----------------------------------------------------------------
void free_graph(graph_t **graph)
{
    if (graph == NULL || *graph == NULL) {
    return;
    }
if ((*graph)->edges != NULL) {
    free((*graph)->edges);
}
 free(*graph);
 *graph = NULL;
}

// - function -----------------------------------------------------------------
void load_txt(const char *fname, graph_t *graph)
{
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        return;
    }
    while (1) {
        if (graph->num_edges >= graph->capacity) {
            graph->capacity *= 2;  // Double the capacity
            edge_t *temp = realloc(graph->edges, graph->capacity * sizeof(edge_t));
            if (temp == NULL) {
                fclose(f);
                return;  // Clean exit without corrupting pointers
            }
            graph->edges = temp;
        }
        edge_t *e = &graph->edges[graph->num_edges]; // Get pointer to the next free edge
        int r = fscanf(f, "%d %d %d", &e->from, &e->to, &e->cost);
        if (r == 3) {
            graph->num_edges += 1; // Increment the number of edges
        } else if (r == EOF) {
            break;
        }
        // graph->num_edges++;
    }
    fclose(f);

}

// - function -----------------------------------------------------------------
void load_bin(const char *fname, graph_t *graph)
{
    FILE *f = fopen(fname, "rb");
    if (f == NULL) {
        return;
    }
    graph->num_edges = 0;
    // edge_t *e = graph->edges;
    // fread(&e->from,sizeof(int),1,f);
    // fread(&e->to,sizeof(int),1,f);
    // fread(&e->cost,sizeof(int),1,f);
    while (1) {
        if (graph->num_edges >= graph->capacity) {
            graph->capacity *= 2;  // Double the capacity
            edge_t *temp = realloc(graph->edges, graph->capacity * sizeof(edge_t));
            if (temp == NULL) {
                fclose(f);
                return;  // Clean exit without corrupting pointers
            }
            graph->edges = temp;
        }
        int r = fread (&graph->edges[graph->num_edges], sizeof(edge_t), 1, f);
        if (r != 1) {

            break; // End of file or read error
        }
        graph-> num_edges++;
    }

    fclose(f);

}

// - function -----------------------------------------------------------------
void save_txt(const graph_t * const graph, const char *fname)
{
    FILE *f = fopen(fname, "w");
    if (f == NULL) {
        return;
    }
    edge_t *e = graph->edges;

    for (int i = 0; i < graph->num_edges; i++, e++) {
        fprintf(f, "%i %i %i\n", e ->from, e->to, e->cost);
    }
    fclose(f);
}

// - function -----------------------------------------------------------------
void save_bin(const graph_t * const graph, const char *fname)
{
    FILE *f = fopen(fname, "wb");
    if (f == NULL) {
        return;
    }
    edge_t *e = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        // fwrite(&e->from,sizeof(int),1,f);
        // fwrite(&e->to,sizeof(int),1,f);
        // fwrite(&e->cost,sizeof(int),1,f);
        fwrite(&e[i], sizeof(edge_t), 1, f);
    }
    fclose(f);

}
