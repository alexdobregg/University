#include <stdio.h>
#include <stdlib.h>

#define INF 30000

// Graful si numarul de noduri.
int G[8][8];
int nr = 7;

// Distanta, parintii si coada pentru Dijkstra.
int d_djk[8];
int p_djk[8];
int q[8];
int q_size;

// Distantele si parintii pentru Bellman Ford
int d_bf[8];
int p_bf[8];

int min(int a, int b) {
    return (a < b) ? a : b;
}

void extrage_min(int *q, int *size, int *elem) {
    int i;
    int minim = INF;
    int index = -1;
    // Se gaseste indexul elementului cu distanta minima.
    for (i = 1; i <= *size; i++) {
        if (d_djk[q[i]] < minim) {
            minim = d_djk[q[i]];
            index = i;
        }
    }
    // Este returnat prin referinta in elem.
    *elem = q[index];
    // Elementul este eliminat din coada prin intermediul mutarii elementelor de dupa
    // acesta cu o pozitie in spate.
    for(i = index; i <= (*size) - 1; i++) {
        q[i] = q[i + 1];
    }
    (*size)--;
}


void dijkstra(int src) {
    int i, u, v;
    for (i = 1; i <= nr; i++) {
        p_djk[i] = -1;
        d_djk[i] = INF;
    }
    d_djk[src] = 0;
    for (i = 1; i <= nr; i++) {
        q[i] = i;
        q_size++;
    }
    while (q_size > 0) {
        extrage_min(q, &q_size, &u);
        int found = 0;
        for (v = 1; v <= nr; v++) {
            for (int i = 1; i < q_size; i++) {
                if (q[i] == v) {
                    found = 1;
                    break;
                }
            }
            if (G[u][v] != 0 && found == 1) {
                if (d_djk[v] > d_djk[u] + G[u][v]) {
                    d_djk[v] = d_djk[u] + G[u][v];
                    p_djk[v] = u;
                }
            }
        }
    }
    printf("1. Dijkstra\n");
    for (i = 1; i <= nr; i++) {
        if (p_djk[i] == -1) {
            if (d_djk[i] == INF) {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = INF\tp[%d] = NULL\n", src, i, i, i);
            } else {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = %d\tp[%d] = NULL\n", src, i, i, d_djk[i], i);
            }
        } else {
            if (d_djk[i] == INF) {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = INF\tp[%d] = %d\n", src, i, i, i, p_djk[i]);
            } else {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = %d\tp[%d] = %d\n", src, i, i, d_djk[i], i, p_djk[i]);
            }
        }
    }
}



void bellman_ford(int src) {
    int i, u, v;
    for (i = 1; i <= nr; i++) {
        d_bf[i] = INF;
        p_bf[i] = -1;
    }
    d_bf[src] = 0;
    for (i = 1; i <= (nr - 1); i++) {
        for (u = 1; u <= nr; u++) {
            for (v = 1; v <= nr; v++) {
                if (G[u][v] != 0) {
                    int cost_drum = G[u][v];
                    if (d_bf[v] > d_bf[u] + cost_drum) {
                        d_bf[v] = d_bf[u] + cost_drum;
                        p_bf[v] = u;
                    }
                }
            }
        }
    }

    for (u = 1; u <= nr; u++) {
        for (v = 1; v <= nr; v++) {
            if (G[u][v] != 0) {
                int cost_drum = G[u][v];
                if (d_bf[v] > d_bf[u] + cost_drum) {
                    printf("EROARE: Ciclu negativ");
                    exit(1);
                }
            }
        }
    }

    printf("2. Bellman-Ford\n");
    for (i = 1; i <= nr; i++) {
        if (p_bf[i] == -1) {
            if (d_bf[i] == INF) {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = INF\tp[%d] = NULL\n", src, i, i, i);
            } else {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = %d\tp[%d] = NULL\n", src, i, i, d_bf[i], i);
            }
        } else {
            if (d_bf[i] == INF) {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = INF\tp[%d] = %d\n", src, i, i, i, p_bf[i]);
            } else {
                printf("Sursa = %d\tDestinatie = %d\td[%d] = %d\tp[%d] = %d\n", src, i, i, d_bf[i], i, p_bf[i]);
            }
        }
    }
}

int main () {
    G[1][2] = 3;
    G[1][3] = 4;
    G[1][5] = 1;
    G[1][6] = 10;
    G[1][7] = 3;
    G[2][3] = 2;
    G[3][4] = 2;
    G[3][6] = 2;
    G[5][4] = 3;
    G[5][7] = 1;
    G[6][4] = 2;
    G[7][3] = 1;
    G[7][4] = 10;
    G[7][5] = 2;
    dijkstra(1);
    printf("\n");
    bellman_ford(1);
    return 0;
}