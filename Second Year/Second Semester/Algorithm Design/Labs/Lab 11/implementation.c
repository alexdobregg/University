#include <stdlib.h>
#include <stdio.h>

#define INF 30000

int G[12][12];
int n = 12;

int d_prim[12]; // distanta Prim
int p_prim[12]; // parintii Prim
int q[12]; // coada Prim
int q_size; // dimensiunea cozii Prim

int arbore_kruskal[12]; // arbore Kruskal

int min(int a, int b) {
	return a < b ? a : b;
}

void extrage_min(int *q, int *size, int *elem) {
    int i;
    int minim = INF;
    int index = -1;
    for (i = 1; i <= *size; i++) {
        if (d_prim[q[i]] < minim) {
            minim = d_prim[q[i]];
            index = i;
        }
    }
    *elem = q[index];
    for(i = index; i <= (*size) - 1; i++) {
        q[i] = q[i + 1];
    }
    (*size)--;
}

void prim(int src) {
	printf("1. ALGORITMUL LUI PRIM\n\n");
	int ama_size = 0;
	int AMA[12][2];
	int elem;
	for (int i = 0; i < n; i++) {
		d_prim[i] = INF;
		p_prim[i] = -1;
	}
	d_prim[src] = 0;
	for (int i = 0; i < n; i++) {
		q[q_size] = i;
		q_size++;
	}
	int pas = 0;
	while (q_size > 0) {
		int k;
		extrage_min(q, &q_size, &elem);
		if (p_prim[elem] != -1) {
			printf("%d. Este extras nodul %c - d[%c] = %d; p[%c] = %c.\n", pas++,
			(char) elem + 65, (char) elem + 65, d_prim[elem],
			(char) elem + 65, (char) p_prim[elem] + 65);
		} else {
			printf("%d. Este extras nodul %c - d[%c] = %d; p[%c] = NULL.\n", pas++,
			(char) elem + 65, (char) elem + 65, d_prim[elem],
			(char) elem + 65);
		}
		AMA[ama_size][0] = elem;
		AMA[ama_size++][1] = p_prim[elem];
		for (int j = 0; j < n; j++) {
			if(G[elem][j] != 0 || G[j][elem] != 0) { 
				int d = G[elem][j] + G[j][elem];
				if(d < d_prim[j]) {
					p_prim[j] = elem;
					d_prim[j] = d;
				}
			}
		}
	}
	printf("\nARBORELE MINIM DE ACOPERIRE\n\n");
	for (int i = 1; i < ama_size; i++) {
		printf("%c - %c\n", (char) AMA[i][0] + 65, (char) AMA[i][1] + 65);
	}
}

void update_arbore(int *arbore, int idx1, int idx2) {
	int v1 = arbore[idx1];
	int v2 = arbore[idx2];
	int m = min(v1, v2);
	for(int i = 0; i < n; i++) {
		if(arbore[i] == v1 || arbore[i] == v2) {
			arbore[i] = m;
		}
	}
}

void kruskal() {
	printf("\n\n2. ALGORITMUL LUI KRUSKAL\n\n");
	int muchii_kruskal[50][3];
	int muchii_AMA[50][2];
	int nr_muchii = 0;
	int ama_size = 0;
	for(int i = 0; i < n; i++) {
		arbore_kruskal[i] = i;
		for(int j = 0; j < n; j++) {
			if(G[i][j] != 0) {
				muchii_kruskal[nr_muchii][0] = i;
				muchii_kruskal[nr_muchii][1] = j;
				muchii_kruskal[nr_muchii++][2] = G[i][j];
			}
		}
	}
	for(int i = 0; i < nr_muchii; i++) {
		int min = INF;
		int index_min = -1;
		for(int j = 0; j < nr_muchii; j++) {
			if(muchii_kruskal[j][2] < min) {
				min = muchii_kruskal[j][2];
				index_min = j;
			}
		}
		int u = muchii_kruskal[index_min][0];
		int v = muchii_kruskal[index_min][1];
		printf("%d. Muchia %c - %c are costul %d - ", i, (char)u + 65, (char)v + 65, muchii_kruskal[index_min][2]);
		muchii_kruskal[index_min][2] = INF;
		if(arbore_kruskal[u] != arbore_kruskal[v]) {
			printf("se adauga in AMA\n");
			update_arbore(arbore_kruskal, u, v);
			muchii_AMA[ama_size][0] = u; 
			muchii_AMA[ama_size][1] = v;
			ama_size++;
		} else {
			printf("nu se adauga in AMA (formeaza ciclu)\n");
		}
	}
	printf("\nARBORELE MINIM DE ACOPERIRE\n\n");
	for(int i = 0; i < ama_size; ++i) {
		printf("%c - %c\n", (char) muchii_AMA[i][0] + 65,
					(char) muchii_AMA[i][1] + 65);
	}
}

int main() {
	G[0][8] = 3;
	G[0][1] = 9;
	G[0][6] = 10;
	G[0][7] = 6;
	G[1][6] = 11;
	G[1][2] = 5;
	G[2][3] = 8;
	G[2][4] = 12;
	G[3][5] = 9;
	G[4][5] = 2;
	G[6][7] = 4;
	G[8][9] = 5;
	G[9][10] = 14;
	G[10][11] = 7;
	G[8][11] = 8;
	prim(8);
	kruskal();
	return 0;
}