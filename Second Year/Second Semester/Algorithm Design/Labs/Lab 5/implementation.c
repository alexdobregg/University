#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Problema 2: k-garduri.

void k_fence(int n, int k) {
    // K e 2 deci am folosit i - 2 si =.
    char sols[n + 1][100][100];
    int nrSol[n + 1];
    int solI = 0;

    int D[n + 1];
    D[0] = 1;
    D[1] = 1;

    strcpy(sols[1][0], "|");
    nrSol[0] = 1;

    for (int i = 2; i <= n; i++) {
        solI = 0;
            if (i == 2) {
                strcat(sols[i][solI++], "||");
                strcpy(sols[i][solI++], "=");
                nrSol[i] = solI;
            } else {
                for(int j = 0; j < nrSol[i - 1]; j++) {
                    char aux[50];
                    strcpy(aux, sols[i - 1][j]);
                    strcat(aux, "|");
                    strcpy(sols[i][solI++], aux);
                }
                for (int j = 0; j < nrSol[i - 2]; j++) {
                    char aux[50];
                    strcpy(aux, sols[i - 2][j]);
                    strcat(aux, "=");
                    strcpy(sols[i][solI++], aux);
                }
                nrSol[i] = solI;
            }


        D[i] = D[i - 1] + D[i - k];
    }
    printf("Numarul solutiilor este: %d\n", D[n]);
    printf("Acestea sunt:\n");

    for (int i = 0; i < nrSol[n]; i++) {
        printf("%s ", sols[n][i]);
    }
    printf("\n\n");
}


// Problema 3: Problema sume.

void subsetSums(int v[], int n) {
    int nrSubsets = 1 << n;
 	int sum = 0;
 	int last = 0;
    for (int i = 0; i < nrSubsets; i++) {
        sum = 0;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                sum += v[j];
                last = j;
            }
        }
        if(sum % 3 == 0 && sum!=0) {
        	for (int j = 0; j < n; j++) {
            	if (i & (1 << j)) {
            		if (j != last) {
            			printf("%d + ", v[j]);
                    } else {
            			printf("%d ", v[j]);
                    }
            	}
            }
            printf("= %d\n", sum);
        }
    }
}


void sume(int *vector, int n, int impartitor, int rest) {
    // Impartitorul este 3, restul este 0.
    int DP[n + 1][3];
    DP[0][0] = 0;
    DP[0][1] = 0;
    DP[0][2] = 0;

    for (int i = 1; i <= n; i++) {
        if (vector[i - 1] % impartitor == rest) {
            DP[i][0] = 1 + 2 * DP[i - 1][0];
            DP[i][1] = 2 * DP[i - 1][1];
            DP[i][2] = 2 * DP[i - 1][2];
        } else if(vector[i - 1] % impartitor == rest + 1) {
            DP[i][0] = DP[i - 1][0] + DP[i - 1][2];
            DP[i][1] = 1 + DP[i - 1][1] + DP[i - 1][0];
            DP[i][2] = DP[i - 1][2] + DP[i - 1][1];
        } else if(vector[i - 1] % impartitor == rest + 2) {
            DP[i][0] = DP[i - 1][0] + DP[i - 1][1];
            DP[i][1] = DP[i - 1][1] + DP[i - 1][2];
            DP[i][2] = 1 + DP[i - 1][2] + DP[i - 1][0];
        }
    }
    subsetSums(vector, n);
    printf("Numarul de sume este: %d\n", DP[n][0]);
}

int main() {
    printf("2. PROBLEMA K-GARDURI:\n");
    int n2 = 6, k2 = 2;
    k_fence(n2, k2);

    printf("3. PROBLEMA SUME: \n");
    int v3[] = {3, 1, 2, 4};
    int n3 = 4;
    sume(v3, n3, 3, 0);
    return 0;
}
