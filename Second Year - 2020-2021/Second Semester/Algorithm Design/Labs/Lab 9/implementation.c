#include <stdio.h>
#include <stdlib.h>

// Numarul de noduri si graful pe care il vom folosi in problema.
int nr = 10;
int G[10][10];

// Folosite la articulatii.
int low_art[10]; // low
int subarb_art[10]; // subarb
int art[10]; // art
int d_art[10]; // d
int p_art[10]; // p
int c_art[10]; // c
int timp_art; // variabila timp
int nr_art; // numarul de articulatii


// Folosite la punti.
int low_punti[10]; // low
int punte[10][2]; // punte
int d_punti[10]; // d
int p_punti[10]; // p
int c_punti[10]; // c
int timp_punti; // variabila timp
int nr_punti; // nr de punti


int min(int a, int b) {
  return a < b ? a : b;
}

// PROBLEMA 1 - ARTICULATII.
void exploreaza_art(int u) {
  d_art[u] = low_art[u] = timp_art++; // initializari
  c_art[u] = 1; // gri
  int v;
  for (v = 0; v < nr; v++) {
    if (G[u][v] == 1 || G[v][u] == 1) {
      if (c_art[v] == 0) {
        p_art[v] = u;
        subarb_art[u]++; // numarul de subarbori dominati de u este actualizat
        exploreaza_art(v);
        low_art[u] = min(low_art[u], low_art[v]); // low este actualizat
        if (p_art[u] != -1 && low_art[v] >= d_art[u]) {
          art[u] = 1; // cazul II al teoremei
          nr_art++;
        }
      } else if (p_art[u] != v) {
        low_art[u] = min(low_art[u], d_art[v]); // low este actualizat
      }
    }
  }
}

// ALGORITMUL LUI TARJAN PENTRU ARTICULATII.
void articulatii_tarjan() { 
  for (int u = 0; u < nr; u++) {
    p_art[u] = -1; // parintele default
    c_art[u] = 0; // culoarea alb
    d_art[u] = 0; // distanta initiala
    low_art[u] = 0; // low-ul initial
    subarb_art[u] = 0; // numarul de subarbori dominati de u initial
    art[u] = 0; // punctele de articulatie
  }

  nr_art = 0;
  timp_art = 0;

  for (int u = 0; u < nr; u++) {
    if (c_art[u] == 0) { 
      exploreaza_art(u);
      if (subarb_art[u] > 1) {
        art[u] = 1; // cazul I al teoremei
        nr_art++;
      }
    }
    c_art[u] = 2; // culoarea negru
  }

  printf("Punctele de articulatie (%d) sunt: ", nr_art);
  for (int u = 0; u < nr; u++) {
    if (art[u] == 1) {
      printf("%d ", u);
    }
  }
  printf("\n\n");
}

// PROBLEMA 2 - PUNTI.
void exploreaza_punti(int u) {
  d_punti[u] = low_punti[u] = timp_punti++; // initializari
  c_punti[u] = 1; // culoarea gri
  for (int v = 0; v < nr; v++) {
    if (G[u][v] == 1 || G[v][u] == 1) {
      if (c_punti[v] == 0) {
        p_punti[v] = u;
        exploreaza_punti(v);
        low_punti[u] = min(low_punti[u], low_punti[v]); // low este actualizat
        if (low_punti[v] > d_punti[u]) {
          punte[nr_punti][0] = v;
          punte[nr_punti][1] = u;
          nr_punti++;
        }
      } else if (p_punti[u] != v) {
        low_punti[u] = min(low_punti[u], d_punti[v]); // loww este actualizat
      }
    }
  }
}
// ALGORITMUL LUI TARJAN PENTRU PUNTI.
void punti_tarjan() { // functia care determina puntile dintr-un graf
  for (int u = 0; u < nr; u++) {
    p_punti[u] = -1; // parintele default
    c_punti[u] = 0; // culoarea alb
    d_punti[u] = 0; // distanta initiala
    low_punti[u] = 0; // low-ul initial
    punte[u][0] = punte[u][1] = 0; // puntea initiala
  }

  nr_punti = 0;
  timp_punti = 0;

  for (int u = 0; u < nr; ++u) {
    if (c_punti[u] == 0) {
      exploreaza_punti(u);
    }
    c_punti[u] = 2; // culoarea negru
  }

  printf("Puntile (%d) sunt:\n", nr_punti);
  for (int i = 0; i < nr_punti; ++i) {
    printf("%d - %d\n", punte[i][0], punte[i][1]);
  }
}

int main() {
  G[0][1] = 1;
  G[0][9] = 1;
  G[1][2] = 1;
  G[1][8] = 1;
  G[2][3] = 1;
  G[2][4] = 1;
  G[2][7] = 1;
  G[3][4] = 1;
  G[3][5] = 1;
  G[5][6] = 1;
  G[7][8] = 1;

  printf("1. PUNCTE DE ARTICULATIE\n");
  articulatii_tarjan();
  printf("2. PUNTI\n");
  punti_tarjan();
  return 0;
}