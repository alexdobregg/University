// Dobre Gigi-Alexandru 312CD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "functions.h"
#include "CitCr.h"

void task1(FILE *fileIN, FILE *fileOUT) {
    int i, nrFilme, nrActtot;
    // Citim numarul de filme.
    fscanf(fileIN, "%d", &nrFilme);
    // Alocam vectorul ce tine minte numarul de actori pentru fiecare film.
    int *nrAct = (int *) calloc(nrFilme, sizeof(int));
    if(!nrAct) exit(1);
    // Obtinem matricea cu numele actorilor.
    char ***numeActori = citire(fileIN, nrFilme, nrAct, &nrActtot);
    // Obtinem graful reprezentat prin liste ade adiacenta.
    TGraphL *graph = cr_graph(nrFilme, nrAct, nrActtot, numeActori);
    int *val = (int *) calloc(graph->nn, sizeof(int));
    int max = 0;
    // Obtinem cel mai mare subgraf conex.
    char **SubG = dfs(graph, val, &max);
    fprintf(fileOUT, "%d\n", max);
    for(i = 0; i < max; i++) {
        fprintf(fileOUT, "%s", SubG[i]);
    }
    // Eliberam memoria.
    for(i = 0; i < max; i++) {
        free(SubG[i]);
    }
    free(SubG);
    destroyGraphAdjList(graph);
    for(i = 0; i < nrFilme; i++) {
        for(int j = 0; j < nrAct[i]; j++) {
            free(numeActori[i][j]);
        }
        free(numeActori[i]);
    }
    free(numeActori);
    free(nrAct);
    free(val);
}

void task2(FILE *fileIN, FILE *fileOUT) {
    int i, v1, v2, nrFilme, nrActtot;
    // Citim numarul de filme.
    fscanf(fileIN, "%d", &nrFilme);
    // Alocam vectorul ce tine minte numarul de actori pentru fiecare film.
    int *nrAct = (int *) calloc(nrFilme, sizeof(int));
    if(!nrAct) exit(1);
    // Obtinem matricea cu numele actorilor.
    char ***numeActori = citire(fileIN, nrFilme, nrAct, &nrActtot);
    // Alocam memorie pentru numele celor doi Actori.
    char *Actor1 = (char *) calloc(N, sizeof(char));
    if(!Actor1) exit(1);
    char *Actor2 = (char *) calloc(N, sizeof(char));
    if(!Actor2) exit(1);
    fgets(Actor1, N, fileIN);
    fgets(Actor2, N, fileIN);
    // Obtinem graful reprezentat prin liste ade adiacenta.
    TGraphL *graph = cr_graph(nrFilme, nrAct, nrActtot, numeActori);
	v1 = retNod(graph, Actor1);
	v2 = retNod(graph, Actor2);
    int cost = dijkstra(*graph, v1, v2);
    fprintf(fileOUT, "%d\n", cost);
    // Eliberam memoria.
    free(Actor1);
    free(Actor2);
    destroyGraphAdjList(graph);
    for(i = 0; i < nrFilme; i++) {
        for(int j = 0; j < nrAct[i]; j++) {
            free(numeActori[i][j]);
        }
        free(numeActori[i]);
    }
    free(numeActori);
    free(nrAct);
}

void task3(FILE *fileIN, FILE *fileOUT) {
    int i, nrFilme, nrActtot;
    // Citim numarul de filme.
    fscanf(fileIN, "%d", &nrFilme);
    // Alocam vectorul ce tine minte numarul de actori pentru fiecare film.
    int *nrAct = (int *) calloc(nrFilme, sizeof(int));
    if(!nrAct) exit(1);
    // Obtinem matricea cu numele actorilor.
    char ***numeActori = citire(fileIN, nrFilme, nrAct, &nrActtot);
    // Obtinem graful reprezentat prin liste ade adiacenta.
    TGraphL *graph = cr_graph(nrFilme, nrAct, nrActtot, numeActori);
    punti(graph, fileOUT);
    // Eliberam memoria.
    destroyGraphAdjList(graph);
    for(i = 0; i < nrFilme; i++) {
        for(int j = 0; j < nrAct[i]; j++) {
            free(numeActori[i][j]);
        }
        free(numeActori[i]);
    }
    free(numeActori);
    free(nrAct);
}

int main(int argc, char *argv[]) {
    // Deschidem fisierule de input si de output.
    FILE *fileIN = fopen(argv[2], "rt");
    FILE *fileOUT = fopen(argv[3], "wt");
    // Depinzand de cerinta, este apela una dintre functiile pentru taskuri.
    if(strcmp(argv[1], "-c1") == 0) {
        task1(fileIN, fileOUT);
    } else if(strcmp(argv[1], "-c2") == 0) {
        task2(fileIN, fileOUT);
    } else if(strcmp(argv[1], "-c3") == 0) {
        task3(fileIN, fileOUT);
    }
    // Inchidem fisierele.
    fclose(fileIN);
    fclose(fileOUT);
    return 0;
}