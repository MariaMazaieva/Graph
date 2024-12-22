#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"

/* This is a testing file just to be able to compile the 
 * examples of binary files to convert graph from txt->bin
 * and bin -> txt
 */

// - function -----------------------------------------------------------------
graph_t* allocate_graph(void)
{
    graph_t* g = (graph_t*)malloc(sizeof(graph_t));
    if (!g) return NULL;

    g->num_edges = 0;
    g->capacity = 1000;
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
    if (!f) return;

    char number;
    int num_word = 0;
    int from = 0;
    int to = 0;
    int cost = 0;

    while ((number = getc_unlocked(f)) != EOF) {
        /*handling dynamic reallocation*/
        if (graph->num_edges >= graph->capacity) {
            graph->capacity += graph->capacity / 2; //had idea from the internet, previous was *2
            edge_t *temp = realloc(graph->edges, graph->capacity * sizeof(edge_t));
            if (temp == NULL) {
                fclose(f);
                return;
            }
            graph->edges = temp;
        }
        /*have to make sure the number is in the range*/
        if (number >='0' && number <='9') {
            number = number - '0';
            switch(num_word) {
                case 0:
                    from = from * 10 + number;break;
                case 1:
                    to = to* 10 + number;break;
                case 2:
                    cost =  cost* 10 + number;break;
            }
        }else if (number == ' ' && num_word < 2){//didnt work until had condition num_word
            num_word++;
        }
        /* special condition for a new line */
        else if(number == '\n'){
            if (num_word == 2) { 
                edge_t *e = &graph->edges[graph->num_edges];
                e->from = from;
                e->to = to;
                e->cost = cost;

                graph->num_edges++;
            }
            /*reset the values*/
            from = 0;
            to = 0;
            cost = 0;
            num_word = 0;
        }

    }

    fclose(f);
}

// - function -----------------------------------------------------------------
void load_bin(const char *fname, graph_t *graph)
{
    FILE *f = fopen(fname, "rb");
    if (!f ) return;

    graph->num_edges = 0;
    while (1) {
        if (graph->num_edges >= graph->capacity) {
            graph->capacity *= 2;
            edge_t *temp = realloc(graph->edges, graph->capacity * sizeof(edge_t));
            if (temp == NULL) {
                fclose(f);
                return;
            }
            graph->edges = temp;
        }
        /*size_t fread(void * buffer, size_t size, size_t count, FILE * stream)*/
        int r = fread (&graph->edges[graph->num_edges], sizeof(edge_t), 1, f);
        if (r != 1) {

            break;
        }
        graph-> num_edges++;
    }

    fclose(f);
}

// - function -----------------------------------------------------------------
void save_txt(const graph_t * const graph, const char *fname)
{
    FILE *f = fopen(fname, "w");
    if (!f) return;

    edge_t *e = graph->edges;

    for (int i = 0; i < graph->num_edges; i++, e++) {
        fprintf(f, "%i %i %i\n", e ->from, e->to, e->cost);//first - file, second - elements, third - addresses
    }

    fclose(f);
}

// - function -----------------------------------------------------------------
void save_bin(const graph_t * const graph, const char *fname)
{
    FILE *f = fopen(fname, "wb");
    if (!f) return;

    edge_t *e = graph->edges;

    for (int i = 0; i < graph->num_edges; i++) {
        /*pointer to array, size in BYTES, number of elements, ptr to the FILE (stream)*/
        fwrite(&e[i], sizeof(edge_t), 1, f);
    }
    fclose(f);
}


/*
  *size_t fread(void * buffer, size_t size, size_t count, FILE * stream)
* buffer: It refers to the pointer to the buffer memory block where the data read will be stored.
* size: It refers to the size of each element in bytes.
* count: It refers to the count of elements to be read.
* stream: It refers to the pointer to the file stream.
 */




