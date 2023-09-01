#ifndef GRAPH_H_
#define GRAPH_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100

typedef struct TNode {
	// Eticheta nodului.
	int v;
	int cost;
    char *nume;
	// Link catre urmatorul nod din lista de adiacenta.
	struct TNode *next;
} TNode;

typedef struct TGraphL {
	// Numarul de noduri.
	int nn;
	TNode **adl;
} TGraphL;

TGraphL *createGraphAdjList(int numberOfNodes) {
	// Alocam memorie pentru graf.
	TGraphL *graph = (TGraphL *) calloc(1, sizeof(TGraphL));
	if(!graph) exit(1);
	graph->nn = numberOfNodes;
	// Alocam memorie pentru noduri.
	graph->adl = (TNode **) calloc(numberOfNodes, sizeof(TNode *));
	for (int i = 0; i < numberOfNodes; i++) {
		/* Alocam memorie pentru fiecare nod, pentru nume si setam
		campurile corespunzator. */
		graph->adl[i] = (TNode *) calloc(1, sizeof(TNode));
		graph->adl[i]->nume = (char *) calloc(N, sizeof(char));
        graph->adl[i]->next = NULL;
        graph->adl[i]->cost = 1;
        graph->adl[i]->v = i;
	}
	return graph;
}

void addEdgeList(TGraphL* graph, int v1, char *nume, int v2) {
	/* Alocam memorie pentru un nod din lista de adiacenta si completam
	campurile. */
	TNode *nod = (TNode *) calloc(1, sizeof(TNode));
	nod->nume = nume;
	nod->cost = 1;
	nod->v = v2;
	// Il introducem in lista.
	TNode *it = graph->adl[v1];
	while(it->next != NULL) {
		it = it->next;
	}
	it->next = nod;
	nod->next = NULL;
}


void destroyGraphAdjList(TGraphL *graph) {
	int i;
	// Se elibereaza memoria pentru un graf.
	for (i = 0; i < graph->nn; i++) {
		TNode *it = graph->adl[i];
		free(graph->adl[i]->nume);
		while(it != NULL) {
			TNode *aux = it;
			it = it->next;
			free(aux);
		}
	}
	free(graph->adl);
	free(graph);
}

#endif /* GRAPH_H_ */