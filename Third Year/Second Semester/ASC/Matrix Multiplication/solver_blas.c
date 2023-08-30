/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"
#include <string.h>
#include <cblas.h>

/* 
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A, double *B) {
	/* Allocating memory for the final matrix and the aux one. */
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
	// /* C_aux = A x B */
	cblas_dcopy(N * N, B, 1, C_aux, 1);
	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, N, N, 1.0, A, N, C_aux, N);
	
	/* C = C_aux * A' */
	cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasTrans, CblasNonUnit, N, N, 1.0, A, N, C_aux, N);
	cblas_dcopy(N * N, C_aux, 1, C, 1);

	/* C += B' x B' */
	cblas_dgemm(CblasRowMajor, CblasTrans, CblasTrans, N, N, N, 1.0, B, N, B, N, 1.0, C, N);

	free(C_aux);
	return C;
}
