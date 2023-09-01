#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "heap.h"
#include <limits.h>
#define Inf 10000


// TASK 1

// Functie pentru qsort.
int string_cmp(const void *a, const void *b) {
    const char **ia = (const char **) a;
    const char **ib = (const char **) b;
    return strcmp(*ia, *ib);
}

void visit(TGraphL *graph, int k, int *val, int *count, int *j, char ***numeG) {
	TNode *t;
	int ind = 0;
	val[k] = ++ind;
	// Iteram prin vecinii nodului transmis functiei pentru a construi subgraful.
	for(t = graph->adl[k]; t != NULL; t = t->next) {
		if (val[t->v] == 0) {
			strcpy((*numeG)[(*j)], graph->adl[t->v]->nume);
			(*j)++;
			visit(graph, t->v, val, &(*count), &(*j), &(*numeG));
		}
	}
	(*count)++;
}

char **dfs(TGraphL *graph, int *val, int *max) {
	int i, j;
	int count = 0;
	int nr = 0;
	/* Alocam memorie pentru vectorul in care este retinut numarul de noduri
	pentru fiecare subgraf conex. */
	int *nrCon = (int *) calloc(N, sizeof(int));
	if(!nrCon) exit(1);
	/* Alocam memorie pentru vectorul in care vom tine minte elementele din
	fiecare subgraf in ordine incepand de la primul subgraf. */
	char **numeG = (char **) calloc(graph->nn, sizeof(char *));
	if(!numeG) exit(1);
	for(i = 0; i < graph->nn; i++) {
		numeG[i] = (char *) calloc(N, sizeof(char));
		if(!numeG) exit(1);
	}
	// Este aplicat dfs pentru a obtine componentele conexe.
	for(i = 0; i < graph->nn; i++) {
		val[i] = 0;
	}
	j = 0;
	for(i = 0; i < graph->nn; i++) {
		if(val[i] == 0) {
			strcpy(numeG[j], graph->adl[i]->nume);
			j++;
			visit(graph, i, val, &count, &j, &numeG);
			nrCon[nr++] = count;
			count = 0;
		}
	}
	// Obtinem subgraful cu cele mai multe noduri.
	int idx;
	for(i = 0; i < nr; i++) {
		if((*max) < nrCon[i]) {
			(*max) = nrCon[i];
			idx = i;
		}
	}
	char **SubG = (char **) calloc((*max), sizeof(char *));
	if(!SubG) exit(1);
	for(i = 0; i < (*max); i++) {
		SubG[i] = (char *) calloc(N, sizeof(char));
		if(!SubG[i]) exit(1);
	}
	int inceput = 0, fin = 0;
	if(idx == 0) {
		inceput = 0;
	} else {
		for(i = 1; i <= idx; i++) {
			inceput += nrCon[i - 1];
		}
	}	
	fin = inceput + nrCon[idx];
	int k = 0;
	for(i = inceput; i < fin; i++) {
		strcpy(SubG[k++], numeG[i]);
	}
	// Eliberam memoria si sortam subgraful care ne intereseaza.
	for(i = 0; i < graph->nn; i++) {
		free(numeG[i]);
	}
	free(numeG);
	free(nrCon);
	qsort(SubG, (*max), sizeof(char *), string_cmp);
	return SubG;
}

// END TASK 1

// TASK 2

int dijkstra(TGraphL G, int s, int end) {
    int v, i;
	int V = G.nn;
	// Distanta.
	int d[V];
	// Se construieste min heap.
	MinHeap *h = newQueue(V);
	// Se pun elementele din graf in heap.
	for(v = 0; v < V; v++) {
		// Infinit.
		d[v] = INT_MAX;
		h->elem[v] = newNode(v, d[v]);
		h->pos[v] = v;
	}
	// Se seteaza elementul de start in heap.
	d[s] = 0;
	SiftUp(h, s, d[s]);
	h->size = V;
	while(!isEmpty(h)) {
		MinHeapNode *MinHeapNode = removeMin(h);
		int u = MinHeapNode->v;
		TNode *it = G.adl[u];
		// Merge prin lista de adiacenta pentru nod.
		while(it) {
			int v = it->v;
			if(isInMinHeap(h, v) && d[u] + it->cost < d[v]) {
				d[v] = d[u] + it->cost;
				SiftUp(h, v, d[v]);
			}
			it = it->next;
		}
		free(MinHeapNode);
	}
	// Daca nu exista path de la sursa la nod, vom face distanta -1.
	for(i = 0; i < G.nn; i++) {
		if((INT_MAX <= d[i] || d[i] <= -INT_MAX)) {
			d[i] = -1;
		}
	}
	// Eliberam memoria in plus si returnam distanta care ne intereseaza.
	free(h->elem);
	free(h->pos);
	free(h);
	return d[end];
}

// Functie care returneaza nodul aferent unui nume.
int retNod(TGraphL *graph, char *nume) {
	int i, nod;
	for(i = 0; i < graph->nn; i++) {
		if(strcmp(nume, graph->adl[i]->nume) == 0) {
			nod = graph->adl[i]->v;
			break;
		}
	}
	return nod;
}

// END TASK 2

// TASK 3

// Functie de minim.
int min(int a, int b) {
	if(a < b) {
		return a;
	} else {
		return b;
	}
}


void dfsB(TGraphL *graph, int i, int *time, int **idx, int **low, int **pi,
									 int *nrpunti, char ***numepunti, int *k) {
	(*idx)[i] = (*time);
	(*low)[i] = (*time);
	(*time)++;
	TNode *it;
	// Cautam puntile din graf.
	for(it = graph->adl[i]; it != NULL; it = it->next) {
		if(it->v != (*pi)[i]) {
			if((*idx)[it->v] == -1) {
				(*pi)[it->v] = i;
				dfsB(graph, it->v, &(*time), &(*idx), &(*low), &(*pi),
									 &(*nrpunti), &(*numepunti), &(*k));
				(*low)[i] = min((*low)[i], (*low)[it->v]);
				if((*low)[it->v] > (*idx)[i]) {
					/* Daca este gasita o punte, obtinem numele celor care
					fac parte din ea si crestem numarul puntilor. */
					(*nrpunti)++;
					strcpy((*numepunti)[*k], graph->adl[i]->nume);
					strcpy((*numepunti)[(*k) + 1], graph->adl[it->v]->nume);
					(*k) = (*k) + 2;
				}
			} else {
				(*low)[i] = min((*low)[i], (*idx)[it->v]);
			}
		}
	} 
}

void punti(TGraphL *graph, FILE *fileOUT) {
	int i, time = 0;
	int nrpunti = 0;
	/* Alocam memoria pentru vectorul de punti in care retinem
	actorii inainte de ai ordona in cadrul unei perechi. */
	char **numePunti = (char **) calloc(Inf, sizeof(char *));
	if(!numePunti) exit(1);
	for(i = 0; i < Inf; i++) {
		numePunti[i] = (char *) calloc(N, sizeof(char));
		if(!numePunti[i]) exit(1);
	}
	/* Alocam memorie pentru variabilele necesare algoritmului
	si le setam la -1, Inf(definit ca 10000) si 0. */
	int *idx = (int *) calloc(graph->nn, sizeof(int));
	if(!idx) exit(1);
	int *low = (int *) calloc(graph->nn, sizeof(int));
	if(!low) exit(1);
	int *pi = (int *) calloc(graph->nn, sizeof(int));
	if(!pi) exit(1);
	for(i = 0; i < graph->nn; i++) {
		idx[i] = -1;
		low[i] = Inf;
		pi[i] = 0;
	}
	/* Apelam dfsB care ne va intoarce numarul de punti si numele celor
	care face parte din fiecare punte. */
	int k = 0;
	for(i = 0; i < graph->nn; i++) {
		if(idx[i] == -1) {
			dfsB(graph, i, &time, &idx, &low, &pi, &nrpunti, &numePunti, &k);
		}
	}
	/* Alocam memoria pentru vectorul pe care il vom construi punand fiecare punte
	pe acelasi rand, fiecare pereche fiind ordonata. */
	char **Punti = (char **) calloc(nrpunti, sizeof(char *));
	if(!Punti) exit(1);
	for(i = 0; i < nrpunti; i++) {
		Punti[i] = (char *) calloc(2 * N, sizeof(char));
		if(!Punti[i]) exit(1);
	}
	k = 0;
	for(i = 0; i < nrpunti; i++) {
		if(strcmp(numePunti[k], numePunti[k + 1]) < 0) {
			strcpy(Punti[i], numePunti[k]);
			Punti[i][strlen(numePunti[k] + 1)] = '\0';
			strcat(Punti[i], " ");
			strcat(Punti[i], numePunti[k + 1]);
			k = k + 2;
		} else {
			strcat(Punti[i], numePunti[k + 1]);
			Punti[i][strlen(numePunti[k + 1] + 1)] = '\0';
			strcat(Punti[i], " ");
			strcat(Punti[i], numePunti[k]);
			k = k + 2;
		}
	}
	// Ordonam si puntile, folosind qsort.
	qsort(Punti, nrpunti, sizeof(char *), string_cmp);
	fprintf(fileOUT, "%d\n", nrpunti);
	for(i = 0; i < nrpunti; i++) {
		fprintf(fileOUT, "%s", Punti[i]);
	}
	// Eliberam memoria.
	for(i = 0; i < nrpunti; i++) {
		free(Punti[i]);
	}
	free(Punti);
	for(i = 0; i < Inf; i++) {
		free(numePunti[i]);
	}
	free(numePunti);
	free(idx);
	free(low);
	free(pi);
}

// END TASK 3