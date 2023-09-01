#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Problema 1 -> Generarea permutarilor.

int check_valid(int *v, int i) {
    for (int j = 0; j < i; j++) {
        if (v[j] == v[i]) {
            return 0;
        }
    }
    return 1;
}

void permutariBT(int n) {
    int i, j;
    int *v = (int *) malloc(n * sizeof(int));
    int valid;
    int max = n;
    for (i = 0; i < n; i++) {
        v[i] = 0;
    }
    i = 0;
    printf("Solutiile sunt: \n");
    while (i >= 0) {
        valid = 0;
        while (valid == 0 && v[i] <= max) {
            v[i]++;
            valid = check_valid(v, i);
        }
        if (v[i] <= max) {
            if (i == n - 1) {
                for (j = 0; j < n; j++) {
                    printf("%d ", v[j]);
                }
                printf("\n");
            } else {
                i++;
            }
        } else {
            v[i] = 0;
            i--;
        }
    }
    free(v);
}

// Problema 2 -> Generarea submultimilor

void submultimiBT(int n) {
    int i, j;
    int *v = (int *) malloc(n * sizeof(int));
    int max = 1;
    for (i = 0; i < n; i++) {
        v[i] = -1;
    }
    i = 0;
    printf("Solutiile sunt: \n");

    while (i >= 0) {
        v[i]++;
        if (v[i] <= max) {
            if (i == n - 1) {
                for (j = 0; j < n; j++) {
                    printf("%d ", v[j]);
                }
                printf("-> { ");
                for (j = 0; j < n; j++) {
                    if (v[j] == 1 && j != n - 1) {
                        printf("%d ", j + 1);
                    }
                    if (v[j] == 1 && j == n - 1) {
                        printf("%d ", j + 1);
                    }
                }
                printf("}\n");
            } else {
                i++;
            }
        } else {
            v[i] = -1;
            i--;
        }
    }
    free(v);
}


// Problema 3 -> Problema damelor

int check_valid_dame(int *v, int poz){
	for(int i = 0; i <= poz; i++){
		for(int j = 0; j <= poz; j++){
			if(i != j && ((abs(i - j) == abs(v[i] - v[j])) || (v[i] == v[j])))
				return 0;
		}
	}
	return 1;
}

void dameBT(int n) {
    int i, j;
    int *v = (int *) malloc(n * sizeof(int));
    int valid;
    int max = n - 1;
    for (i = 0; i < n; i++) {
        v[i] = -1;
    }
    i = 0;
    printf("Solutiile sunt: \n");
    while (i >= 0) {
        valid = 0;
        while (valid == 0 && v[i] <= max) {
            v[i]++;
            valid = check_valid_dame(v, i);
        }
        if (v[i] <= max) {
            if (i == n - 1) {
                for (j = 0; j < n; j++) {
                    printf("D%d: (%d %d); ", j + 1, j, v[j]);
                }
                printf("\n");
            } else {
                i++;
            }
        } else {
            v[i] = -1;
            i--;
        }
    }
    free(v);
}

int main() {
    printf("1. GENERAREA PERMUTARILOR: \n \n");
    int n1 = 3;
    permutariBT(n1);
    printf("\n");

    printf("2. GENERAREA SUBMULTIMILOR: \n \n");
    int n2 = 3;
    submultimiBT(n2);
    printf("\n");

    printf("3. PROBLEMA DAMELOR: \n \n");
    int n3 = 4;
    dameBT(n3);

    return 0;
}