#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define FILE_IN0 "cluster0.txt"
#define FILE_IN1 "cluster1.txt"
#define FILE_IN2 "cluster2.txt"

#define COORD0 0
#define COORD1 1
#define COORD2 2

#define min(a, b) a < b ? a : b

int *open_read(int coord, int *nrWorkers) {
    FILE *fIn;
    // Opening files for read and getting the number of workers every
    // coordinator has.
    if (coord == COORD0) {
        fIn = fopen(FILE_IN0, "r");
    } else if (coord == COORD1) {
        fIn = fopen(FILE_IN1, "r");
    } else if (coord == COORD2) {
        fIn = fopen(FILE_IN2, "r");
    }

    fscanf(fIn, "%d", &(*nrWorkers));
    int *workers = (int *)calloc((*nrWorkers), sizeof(int));
    for (int i = 0; i < (*nrWorkers); i++) {
        fscanf(fIn, "%d", &workers[i]);
    }
    fclose(fIn);

    return workers;
}

void printing(int rank, int nrWorkers0, int* workers0, int nrWorkers1, int* workers1, int nrWorkers2, int* workers2) {
    // Printing the topology from a specific rank.
    printf("%d -> %d:", rank, COORD0);
    for (int i = 0; i < nrWorkers0; i++) {
        printf("%d", workers0[i]);
        if (i < nrWorkers0 - 1) {
            printf(",");
        } else if ( i == nrWorkers0 - 1) {
            printf(" ");
        }
    }
    printf("%d:", COORD1);
    for (int i = 0; i < nrWorkers1; i++) {
        printf("%d", workers1[i]);
        if (i < nrWorkers1 - 1) {
            printf(",");
        } else if ( i == nrWorkers1 - 1) {
            printf(" ");
        }
    }
    printf("%d:", COORD2);
    for (int i = 0; i < nrWorkers2; i++) {
        printf("%d", workers2[i]);
        if (i < nrWorkers2 - 1) {
            printf(",");
        } else if ( i == nrWorkers2 - 1) {
            printf("\n");
        }
    }
}

void sendToWorker(int rank, int worker, int nrWorkers0, int* workers0, int nrWorkers1, int* workers1, int nrWorkers2, int* workers2) {
    // Sending the topology to a specific worker from his coordinator.
    MPI_Send(&nrWorkers0, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker);
    MPI_Send(workers0, nrWorkers0, MPI_INT, worker, 0, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker);

    MPI_Send(&nrWorkers1, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker);
    MPI_Send(workers1, nrWorkers1, MPI_INT, worker, 0, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker);

    MPI_Send(&nrWorkers2, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker);
    MPI_Send(workers2, nrWorkers2, MPI_INT, worker, 0, MPI_COMM_WORLD);
    printf("M(%d,%d)\n", rank, worker);
}

void rel01(int rank, int vecDim, int nProcesses) {
    if(rank == COORD0) {
        int nrWorkers0;
        int *workers0 = open_read(rank, &nrWorkers0);

        // Sending to the workers the rank of their coordinator.
        for (int i = 0; i < nrWorkers0; i++) {
            MPI_Send(&rank, 1, MPI_INT, workers0[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD0, workers0[i]);
        }

        // Sending to the other 2 coordinators the current's rank topology.
        MPI_Send(&nrWorkers0, 1, MPI_INT, COORD1, 10, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD1);
        MPI_Send(workers0, nrWorkers0, MPI_INT, COORD1, 10, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD1);

        MPI_Send(&nrWorkers0, 1, MPI_INT, COORD2, 20, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD2);
        MPI_Send(workers0, nrWorkers0, MPI_INT, COORD2, 20, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD2);

        // Receiving from the other 2 coordinators their topology.
        int nrWorkers1;
        MPI_Recv(&nrWorkers1, 1, MPI_INT, COORD1, 10, MPI_COMM_WORLD, NULL);
        int *workers1 = (int *)calloc(nrWorkers1, sizeof(int));
        MPI_Recv(workers1, nrWorkers1, MPI_INT, COORD1, 10, MPI_COMM_WORLD, NULL);

        int nrWorkers2;
        MPI_Recv(&nrWorkers2, 1, MPI_INT, COORD2, 20, MPI_COMM_WORLD, NULL);
        int *workers2 = (int *)calloc(nrWorkers2, sizeof(int));
        MPI_Recv(workers2, nrWorkers2, MPI_INT, COORD2, 20, MPI_COMM_WORLD, NULL);

        // Printing the complete topology.
        printing(rank, nrWorkers0, workers0, nrWorkers1, workers1, nrWorkers2, workers2);

        // Sending the complete topology to the workers.
        for (int i = 0; i < nrWorkers0; i++) {
            sendToWorker(rank, workers0[i], nrWorkers0, workers0, nrWorkers1, workers1, nrWorkers2, workers2);
        }

        // Making the vector with values equal to their position in rank0.
        int* v = (int *)calloc(vecDim, sizeof(int));
        int* vAux = (int *)calloc(vecDim, sizeof(int));
        for (int i = 0; i < vecDim; i++) {
            v[i] = i;
        }

        // Sending the vector to the other 2 coordinators.
        MPI_Send(&vecDim, 1, MPI_INT, COORD1, 10, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD1);
        MPI_Send(v, vecDim, MPI_INT, COORD1, 10, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD1);

        MPI_Send(&vecDim, 1, MPI_INT, COORD2, 20, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD2);
        MPI_Send(v, vecDim, MPI_INT, COORD2, 20, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD0, COORD2);


        // Sending the subvector to his workers.
        for (int i = 0; i < nrWorkers0; i++) {
            int start = (workers0[i] - 3) * ceil((double) vecDim / (nProcesses - 3));
            int end = min(vecDim, (workers0[i] - 2) * ceil((double) vecDim / (nProcesses - 3)));
            int dim = end - start + 1;
            MPI_Send(&dim, 1, MPI_INT, workers0[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD0, workers0[i]);
            MPI_Send(v + start, dim, MPI_INT, workers0[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD0, workers0[i]);
            
        }

        // Receiving the modified subvector from the workers and computing the new one.
        for (int i = 0; i < nrWorkers0; i++) {
            int start = (workers0[i] - 3) * ceil((double) vecDim / (nProcesses - 3));
            int end = min(vecDim, (workers0[i] - 2) * ceil((double) vecDim / (nProcesses - 3)));
            int dim = end - start + 1;
            MPI_Recv(vAux, dim, MPI_INT, workers0[i], 0, MPI_COMM_WORLD, NULL);
            int count = 0;
            for (int i = start; i < end; i++) {
                v[i] = vAux[count++];
            }
        }

        // Receiving the modified vector from the other 2 coordonators and computing the final
        // vector.
        MPI_Recv(vAux, vecDim, MPI_INT, COORD1, 10, MPI_COMM_WORLD, NULL);
        for (int i = 0; i < vecDim; i++) {
            if (v[i] == i && vAux[i] != 0) {
                v[i] = vAux[i];
            }
        }

        MPI_Recv(vAux, vecDim, MPI_INT, COORD2, 20, MPI_COMM_WORLD, NULL);
        for (int i = 0; i < vecDim; i++) {
            if (v[i] == i && vAux[i] != 0) {
                v[i] = vAux[i];
            }
        }

        // Printing the result.
        sleep(1);
        printf("Rezultat: ");
        for (int i = 0; i < vecDim; i++) {
           printf("%d ", v[i]);
        }
        printf("\n");

        free(workers0);
        free(workers1);
        free(workers2);
        free(v);
        free(vAux);

    } else if (rank == COORD1) {
        int nrWorkers1;
        int *workers1 = open_read(rank, &nrWorkers1);

        // Sending the rank of their coordinator to the workers.
        for (int i = 0; i < nrWorkers1; i++) {
            MPI_Send(&rank, 1, MPI_INT, workers1[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD1, workers1[i]);
        }

        // Sending to the other 2 coordinators the current's rank topology.
        MPI_Send(&nrWorkers1, 1, MPI_INT, COORD0, 10, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD1, COORD0);
        MPI_Send(workers1, nrWorkers1, MPI_INT, COORD0, 10, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD1, COORD0);

        MPI_Send(&nrWorkers1, 1, MPI_INT, COORD2, 12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD1, COORD2);
        MPI_Send(workers1, nrWorkers1, MPI_INT, COORD2, 12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD1, COORD2);

        // Receiving from the other 2 coordinators their topology.
        int nrWorkers0;
        MPI_Recv(&nrWorkers0, 1, MPI_INT, COORD0, 10, MPI_COMM_WORLD, NULL);
        int *workers0 = (int *)calloc(nrWorkers0, sizeof(int));
        MPI_Recv(workers0, nrWorkers0, MPI_INT, COORD0, 10, MPI_COMM_WORLD, NULL);

        int nrWorkers2;
        MPI_Recv(&nrWorkers2, 1, MPI_INT, COORD2, 12, MPI_COMM_WORLD, NULL);
        int *workers2 = (int *)calloc(nrWorkers2, sizeof(int));
        MPI_Recv(workers2, nrWorkers2, MPI_INT, COORD2, 12, MPI_COMM_WORLD, NULL);

        // Printing the complete topology.
        printing(rank, nrWorkers0, workers0, nrWorkers1, workers1, nrWorkers2, workers2);

        // Sending the complete topology to the workers.
        for (int i = 0; i < nrWorkers1; i++) {
            sendToWorker(rank, workers1[i], nrWorkers0, workers0, nrWorkers1, workers1, nrWorkers2, workers2);  
        }

        // Receiving the vector from rank0.
        int vectorDim;
        MPI_Recv(&vectorDim, 1, MPI_INT, COORD0, 10, MPI_COMM_WORLD, NULL);
        int* v = (int *)calloc(vectorDim, sizeof(int));
        MPI_Recv(v, vectorDim, MPI_INT, COORD0, 10, MPI_COMM_WORLD, NULL);

        // Sending the subvector to his workers.
        for (int i = 0; i < nrWorkers1; i++) {
            int start = (workers1[i] - 3) * ceil((double) vectorDim / (nProcesses - 3));
            int end = min(vectorDim, (workers1[i] - 2) * ceil((double) vectorDim / (nProcesses - 3)));
            int dim = end - start + 1;
            MPI_Send(&dim, 1, MPI_INT, workers1[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD1, workers1[i]);
            MPI_Send(v + start, dim, MPI_INT, workers1[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD1, workers1[i]);
        }

        // Receiving the modified subvector from the workers and computing a new one.
        int* vAux = (int *)calloc(vectorDim, sizeof(int));
        for (int i = 0; i < nrWorkers1; i++) {
            int start = (workers1[i] - 3) * ceil((double) vectorDim / (nProcesses - 3));
            int end = min(vectorDim, (workers1[i] - 2) * ceil((double) vectorDim / (nProcesses - 3)));
            int dim = end - start + 1;
            MPI_Recv(vAux, dim, MPI_INT, workers1[i], 0, MPI_COMM_WORLD, NULL);
            int count = 0;
            for (int i = start; i < end; i++) {
                v[i] = vAux[count++];
            }
        }

        // Sending the computed vector to rank0.
        MPI_Send(v, vectorDim, MPI_INT, COORD0, 10, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD1, COORD0);

        free(workers0);
        free(workers1);
        free(workers2);
        free(v);
        free(vAux);
        
    } else if (rank == COORD2) {
        int nrWorkers2;
        int *workers2 = open_read(rank, &nrWorkers2);

        // Sending the rank of their coordinator to the workers.
        for (int i = 0; i < nrWorkers2; i++) {
            MPI_Send(&rank, 1, MPI_INT, workers2[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD2, workers2[i]);
        }

        MPI_Send(&nrWorkers2, 1, MPI_INT, COORD0, 20, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD2, COORD0);
        MPI_Send(workers2, nrWorkers2, MPI_INT, COORD0, 20, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD2, COORD0);

        MPI_Send(&nrWorkers2, 1, MPI_INT, COORD1, 12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD2, COORD1);
        MPI_Send(workers2, nrWorkers2, MPI_INT, COORD1, 12, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD2, COORD1);

        // Receiving from the other 2 coordinators their topology.
        int nrWorkers0;
        MPI_Recv(&nrWorkers0, 1, MPI_INT, COORD0, 20, MPI_COMM_WORLD, NULL);
        int *workers0 = (int *)calloc(nrWorkers0, sizeof(int));
        MPI_Recv(workers0, nrWorkers0, MPI_INT, COORD0, 20, MPI_COMM_WORLD, NULL);

        int nrWorkers1;
        MPI_Recv(&nrWorkers1, 1, MPI_INT, COORD1, 12, MPI_COMM_WORLD, NULL);
        int *workers1 = (int *)calloc(nrWorkers1, sizeof(int));
        MPI_Recv(workers1, nrWorkers1, MPI_INT, COORD1, 12, MPI_COMM_WORLD, NULL);

        // Printing the complete topology.
        printing(rank, nrWorkers0, workers0, nrWorkers1, workers1, nrWorkers2, workers2);

        // Sending the complete topology to the workers.
        for (int i = 0; i < nrWorkers2; i++) {
            sendToWorker(rank, workers2[i], nrWorkers0, workers0, nrWorkers1, workers1, nrWorkers2, workers2);
        }

        // Receiving the vector from rank0.
        int vectorDim;
        MPI_Recv(&vectorDim, 1, MPI_INT, COORD0, 20, MPI_COMM_WORLD, NULL);
        int* v = (int *)calloc(vectorDim, sizeof(int));
        MPI_Recv(v, vectorDim, MPI_INT, COORD0, 20, MPI_COMM_WORLD, NULL);

        // Sending the subvector to his workers.
        for (int i = 0; i < nrWorkers2; i++) {
            int start = (workers2[i] - 3) * ceil((double) vectorDim / (nProcesses - 3));
            int end = min(vectorDim, (workers2[i] - 2) * ceil((double) vectorDim / (nProcesses - 3)));
            int dim = end - start + 1;
            MPI_Send(&dim, 1, MPI_INT, workers2[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD2, workers2[i]);
            MPI_Send(v + start, dim, MPI_INT, workers2[i], 0, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", COORD2, workers2[i]);
        }

        // Receiving the modified subvector from the workers and computing a new one.
        int* vAux = (int *)calloc(vectorDim, sizeof(int));
        for (int i = 0; i < nrWorkers2; i++) {
            int start = (workers2[i] - 3) * ceil((double) vectorDim / (nProcesses - 3));
            int end = min(vectorDim, (workers2[i] - 2) * ceil((double) vectorDim / (nProcesses - 3)));
            int dim = end - start + 1;
            MPI_Recv(vAux, dim, MPI_INT, workers2[i], 0, MPI_COMM_WORLD, NULL);
            int count = 0;
            for (int i = start; i < end; i++) {
                v[i] = vAux[count++];
            }
        }

        // Sending the computed vector to rank0.
        MPI_Send(v, vectorDim, MPI_INT, COORD0, 20, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", COORD2, COORD0);


        free(workers0);
        free(workers1);
        free(workers2);
        free(v);
        free(vAux);

    } else if (rank > 2){
        int parent;

        // Receiving their coordinator's rank.
        MPI_Recv(&parent, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL);
        

        // Receiving the topology from the coordinator.
        int nrWorkers0;
        MPI_Recv(&nrWorkers0, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);
        int *workers0 = (int *)calloc(nrWorkers0, sizeof(int));
        MPI_Recv(workers0, nrWorkers0, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);

        int nrWorkers1;
        MPI_Recv(&nrWorkers1, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);
        int *workers1 = (int *)calloc(nrWorkers1, sizeof(int));
        MPI_Recv(workers1, nrWorkers1, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);
        
        int nrWorkers2;
        MPI_Recv(&nrWorkers2, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);
        int *workers2 = (int *)calloc(nrWorkers2, sizeof(int));
        MPI_Recv(workers2, nrWorkers2, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);

        // Printing the complete topology.
        printing(rank, nrWorkers0, workers0, nrWorkers1, workers1, nrWorkers2, workers2);

        // // Receiving the subvector from their coordinator.
        int vectorDim;
        MPI_Recv(&vectorDim, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);
        int* v = (int *)calloc(vectorDim, sizeof(int));
        MPI_Recv(v, vectorDim, MPI_INT, parent, 0, MPI_COMM_WORLD, NULL);

        for (int i = 0; i < vectorDim; i++) {
            v[i] *= 2;
        }

        // Sending back the modified subvector to their coordinator.
        MPI_Send(v, vectorDim, MPI_INT, parent, 0, MPI_COMM_WORLD);
        printf("M(%d,%d)\n", rank, parent);

        free(workers0);
        free(workers1);
        free(workers2);
        free(v);    

    }
}

int main(int argc, char *argv[]) {
    int vecDim = atoi(argv[1]);
    int nProcesses, rank;
    int noRel = atoi(argv[2]);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (!noRel) {
        rel01(rank, vecDim, nProcesses);
    }
    
    MPI_Finalize();
    return 0;
}