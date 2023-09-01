#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

char ***citire(FILE *fileIN, int nrFilme, int *nrAct, int *nrActtot) {
    long i, j;
    char x[500];
    (*nrActtot) = 0;
    /* Alocam memorie pentru matricea tridimensionala. Prima alocare
    este pentru fiecare film. A doua alocare este pentru fiecare actor
    in parte, iar a treia alocare este deoarece este vorba de un string.*/
    char ***numeActori = (char ***) calloc(nrFilme, sizeof(char **));
    if(!numeActori) exit(1);
    for(i = 0; i < nrFilme; i++) {
        // Trecem peste numele filmului.
        fseek(fileIN, 1, SEEK_CUR);
        fgets(x, 500, fileIN);
        // Obtinem numarul de actori pentru fiecare film.
        fscanf(fileIN, "%d", &nrAct[i]);
        fseek(fileIN, 1, SEEK_CUR);
        (*nrActtot) += nrAct[i];
        numeActori[i] = (char **) calloc(nrAct[i], sizeof(char *));
        if(!numeActori[i]) exit(1);
        // Extragem numele fiecarui actor intr-o matrice.
        for(j = 0; j < nrAct[i]; j++) {
            numeActori[i][j] = (char *) calloc(N, sizeof(char));
            if(!numeActori[i][j]) exit(1);
            fgets(numeActori[i][j], 100, fileIN);
        }
    }
    return numeActori;
}

TGraphL *cr_graph(int nrFilme, int *nrAct, int nrActtot, char ***numeActori) {
    int i, j, l, k, ok, nrActori, nrActrp = 0;
    // Obtinem De cate ori apare un actor in filme diferite.
    for(i = 1; i < nrFilme; i++) {
        for (k = 0; k < nrAct[i]; k++) {
            for(j = 0; j < i; j++) {
                ok = 0;
                for(l = 0; l < nrAct[j]; l++) {
                    if(strcmp(numeActori[i][k], numeActori[j][l]) == 0) {
                        ok = 1;
                        nrActrp++;
                        break;
                    } 
                }
                if(ok == 1) {
                    break;
                }
            }
        }
    }
    nrActori = nrActtot - nrActrp;
    // Este creat graful.
    TGraphL *graph = createGraphAdjList(nrActori);
    /* Este alocata matricea in care retinem numarul nodului pentru
    fiecare actor in parte. */
    int **idxAct = (int **) calloc(nrFilme, sizeof(int *));
    if(!idxAct) exit(1);
    for(i = 0; i < nrFilme; i++) {
        idxAct[i] = (int *) calloc(nrAct[i], sizeof(int));
        if(!idxAct[i]) exit(1);
    }
    // Completam numele pentru primul film deoarece nu se repeta niciun actor.
    long m = 0;
    for(j = 0; j < nrAct[0]; j++) {
        strcpy(graph->adl[m]->nume, numeActori[0][j]);
        idxAct[0][j] = m;
        m++;
    }
    /* Verificam daca exista acel actor si in alt film. Daca da, vom seta indicele
    la indicele primei aparitii, daca nu, se va adauga in graf. */
    for(i = 1; i < nrFilme; i++) {
        for(k = 0; k < nrAct[i]; k++) {
            ok = 0;
            for(j = 0; j < i; j++) {
                for(l = 0; l < nrAct[j]; l++) {
                    if(strcmp(numeActori[i][k], numeActori[j][l]) == 0) {
                        ok = 1;
                        idxAct[i][k] = idxAct[j][l]; 
                        break;                       
                    } 
                }
            }
            if(ok == 1) {
                continue;
            } else {
                strcpy(graph->adl[m]->nume, numeActori[i][k]);
                idxAct[i][k] = m;
                m++;
            }
        }
    }
    // Sunt create muchiile dintre noduri.
    for(i = 0; i < nrFilme; i++) {
        for(j = 0; j < nrAct[i]; j++) {
            for(k = 0; k < nrActori; k++) {
                if(strcmp(graph->adl[k]->nume, numeActori[i][j]) == 0) {
                    for(l = 0; l < nrAct[i]; l++) {
                        if(strcmp(graph->adl[k]->nume, numeActori[i][l]) != 0) {
                            addEdgeList(graph, k, numeActori[i][l], idxAct[i][l]);
                        }
                    }
                }
            }
        }
    }
    // Eliberam memoria.
    for(i = 0; i < nrFilme; i++) {
        free(idxAct[i]);
    }
    free(idxAct);
    return graph;
}