/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"

double* my_solver(int N, double *A, double* B) {
	double *C = (double *) calloc(N * N, sizeof(double));
	double *C_aux = (double *) calloc(N * N, sizeof(double));
	if (C == NULL || C_aux == NULL) {
		if (C) {
			free(C);
		}
		if (C_aux) {
			free(C_aux);
		}
		printf("Memory not allocated.\n");
		exit(0);
	}
	
	/* C_aux = A x B */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = i; k < N; k++) {
				C_aux[i * N + j] += A[i * N + k] * B[k * N + j];
			}
		}
	}

	/* C = C_Aux * A' */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = j; k < N; k++) {
				C[i * N + j] += C_aux[i * N + k] * A[j * N + k];
			}
		}
	}

	/* C += B' x B' */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				C[i * N + j] += B[k * N + i] * B[j * N + k];
			}
		}
	}

	free(C_aux);
	return C;
}
