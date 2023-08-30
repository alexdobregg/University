#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int max(int a, int b) {
	return a < b? b : a;
}

int min(int a, int b) {
	return a < b? a : b;
}


// Problema 1 -> Parantezarea unei inmultiri de matrice

void print_optimal(int i,int j, int **s) {
	if (i == j) {
		printf(" A%d ",i);
	} else {
      printf("( ");
      print_optimal(i, s[i][j], s);
      print_optimal(s[i][j] + 1, j, s);
      printf(" )");
   }
}

void PODM(int n, int *p) {
	int m[n][n];
	int **s = (int **) calloc(n, sizeof(int *));

	for (int i = 0; i < n; i++) {
		s[i] = (int *) calloc(n, sizeof(int));
	}


	for (int i = 1; i < n; i++) {
		m[i][i] = 0;
	}

	for (int len = 2; len < n; len++) {
		for (int i = 1; i + len - 1 < n; i++) {
			int j = i + len - 1;
			m[i][j] = INT_MAX;

			for (int k = i; k < j; k++) {
				int new_sol = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
				m[i][j] = min(m[i][j], new_sol);
				if(m[i][j] == new_sol) {
					s[i][j] = k;
				}
			}

			if (len > 2) {
				printf("s[%d][%d] = %d \n", i, j, s[i][j]);
			}
		}
	}

	printf("\nParantezarea optima este: ");
	print_optimal(1, n - 1, s);

	printf("\nAceasta se face in %d de operatii.\n", m[1][n - 1]);
	printf("\nCost Matrix m:\n");

	for(int i = 1; i <= n; i++) {
 		for(int j = i; j < n; j++) {
  			printf("m[%d][%d]: %d\n",i,j,m[i][j]);
 		}
	}

	for (int i = 0; i < n; i++) {
		free(s[i]);
	}
	free(s);
}

// Problema 2 -> Problema rucsacului

void knapsack(int M, int *m, int *v, int n) {
	int dp[n + 1][M + 1];
	int keep[n + 1][M + 1];

	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= M; j++) {
			keep[i][j] = 0;
		}
	}

	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= M; j++) {
			if (i == 0 || j == 0) {
				dp[i][j] = 0;
			} else if (j - m[i - 1] >= 0) {
				int sol_aux = dp[i - 1][j - m[i - 1]] + v[i - 1];
				dp[i][j] = max(dp[i - 1][j], sol_aux);
				if(dp[i][j] == sol_aux) {
					keep[i][j] = 1;
				}
			} else {
				dp[i][j] = dp[i - 1][j];
			}
		}
	}

	printf("Continutul rucscacului este: \n");
	int K = M;
	for (int i = n; i >= 1; i--) {
		if (keep[i][K] == 1) {
			printf(" * Greutate: %d; Valoare: %d\n", m[i - 1], v[i - 1]);
			K -= m[i-1];
		}
	}

	printf("Valoarea greutatilor care au loc intr-un sac de %d este: %d\n", M, dp[n][M]);
}





// Problema 3 -> Lungimea maxima a unui subsir crescator al unui sir dat.
void submax(int n, int *vec) {
	int D[n], subS[n], k, maxval, sol = 1;
	D[0] = 1;

	for (int i = 1; i < n; i++) {
		maxval = 0;

		for (int j = 0; j < i; j++) {
			if(vec[i] > vec[j]) {
				maxval = max(maxval, D[j]);
			}
		}

		D[i] = maxval + 1;
		sol = max(sol, D[i]);
		k = 0;
		
		if(sol == D[i]) {
			for (int j = 0; j < i; j++) {
				if (vec[i] > vec[j]) {
					subS[k++] = vec[j];
				}
			}
			subS[k] = vec[i];
		}
	}

	printf("Lungimea maxima a subsirului crescator este: %d\n", sol);
	printf("Subsirul crescator de lungime maxima este: ");
	for (int i = 0; i < sol; i++) {
		printf("%d ", subS[i]);
	}
	printf("\n");
}




int main() {
	printf("1.Parantezarea optima a inmultirii de matrice:\n\n");
	int p1[] = {2, 4, 3, 5, 2};
	int n1 = 5;
	PODM(n1, p1);
	printf("\n\n");

	printf("2.Problema rucsacului:\n\n");
	int M = 50;
	int m[] = {10, 30, 20};
	int v[] = {60, 120, 100};
	int n2 = 3;
	knapsack(M, m, v, n2);
	printf("\n\n");

	printf("3. Lungimea maxima a unui subsir crescator al unui sir dat:\n\n");
	int vec3[] = {1, 2, 9, 3, 8, 4, 7};
	int n3 = 7;
	submax(n3, vec3);

	return 0;
}