/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"

/* Optimized implementation. */
double* my_solver(int N, double *A, double* B) {
	register int i, j, k;
	register int bi, bj, bk;
	register int blockSize = 40;
	register double *C = (double *) calloc(N * N, sizeof(double));
	register double *C_aux = (double *) calloc(N * N, sizeof(double));
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

	/* C_aux = A x B done in i-k-j order*/
	for (bi = 0; bi < N; bi += blockSize) {
		for (bk = bi; bk < N; bk += blockSize) {
			for(bj = 0; bj < N; bj += blockSize) {
				for (i = 0; i < blockSize; i++) {
					register double *c_aux_row = C_aux + (bi + i) * N;
					register double *a_row = A + (bi + i) * N;
					for (k = 0; k < blockSize; k++) {
						register double *b_row = B + (bk + k) * N;
						register double *a_row_col = a_row + (bk + k);
						for (j = 0; j < blockSize; j++) {
							*(c_aux_row + bj + j) += *a_row_col * *(b_row + bj + j);
						}
					}
				}
			}
		}
	}

	/* C = C_Aux * A' done in i-j-k order */
	for (i = 0; i < N; i++) {
		register double *c_aux_row = C_aux + i * N;
		register double *c_row = C + i * N;
		for (j = 0; j < N; j++) {
			register double *a_transpose_row = A + j * N;
			register double sum = 0.0;
			for (k = j; k < N; k++) {
				sum += *(c_aux_row + k) * *(a_transpose_row + k);
			}
			*(c_row + j) = sum;
		}
	}

	/* C += B' x B' done in i-j-k order*/
	for (bi = 0; bi < N; bi += blockSize) {
		for (bj = 0; bj < N; bj += blockSize) {
			for (bk = 0; bk < N; bk += blockSize) {
				for (i = 0; i < blockSize; i++) {
					register double *c_row = C + (bi + i) * N;
					register double *b_transpose_col = B + bi + i;
					for (j = 0; j < blockSize; j++) {
						register double sum = 0.0;
						register double *b_transpose_row = B + (bj + j) * N;
						for (k = 0; k < blockSize; k++) {
							sum += *(b_transpose_col + (bk + k) * N) * *(b_transpose_row + bk + k);
						}
						*(c_row + bj + j) += sum;
					}
				}
			}
		}
	}
	
	free(C_aux);
	return C;	
}
