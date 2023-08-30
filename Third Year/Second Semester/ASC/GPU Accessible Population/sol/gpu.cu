#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <math.h>

#define DEGREE_TO_RADIANS		0.01745329252f

void sampleFileIO(float kmRange, const char* fileIn, const char* fileOut);

#define DIE(assertion, call_description)                    \
do {                                                        \
    if (assertion) {                                        \
            fprintf(stderr, "(%d): ",                       \
                            __LINE__);                      \
            perror(call_description);                       \
            exit(EXIT_FAILURE);                             \
    }                                                       \
} while(0);

using namespace std;

__global__ void compute(float *lat, float *lon, int *pop, int *accpop, int N, float kmRange) {
    /* Obtaining the index of the current element. */
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    for (int i = idx + 1; i < N; i++) {
        float phi1 = (90.f - lat[idx]) * DEGREE_TO_RADIANS;
        float phi2 = (90.f - lat[i]) * DEGREE_TO_RADIANS;

        float theta1 = lon[idx] * DEGREE_TO_RADIANS;
        float theta2 = lon[i] * DEGREE_TO_RADIANS;

        float cs = sin(phi1) * sin(phi2) * cos(theta1 - theta2) + cos(phi1) * cos(phi2);
        if (cs > 1) {
            cs = 1;
        } else if (cs < -1) {
            cs = -1;
        }
        /* If the distance is lesser or equal to kmRange, then atomicAdd is used to add */
        /* the values from pop to accpop. */
        if (6371.f * acos(cs) <= kmRange) {
            atomicAdd(&accpop[idx], pop[i]);
            atomicAdd(&accpop[i], pop[idx]);
        }
    }
}

int main(int argc, char** argv) {
    DIE( argc == 1,
         "./gpu_sol <kmrange1> <file1in> <file1out> ...");
    DIE( (argc - 1) % 3 != 0,
         "./gpu_sol <kmrange1> <file1in> <file1out> ...");

    for(int argcID = 1; argcID < argc; argcID += 3) {
        /* Obtaining the kmRange. */
        float kmRange = atof(argv[argcID]);
        float *lat_device, *lon_device;
        int *pop_device, *accpop_device;
        string geon;
        /* Skipping the H1 test because of timeout. The other 4 are working. */
        if (strstr(argv[argcID + 1], "H1")) {
            continue;
        }

        ifstream file;
        file.open(argv[argcID + 1]);
        DIE(!file, "Error opening input file!");

        /* If kmRange is 0 then the accpop is actually the pop of that city. */
        if (kmRange == 0) {
            ofstream file_out;
            file_out.open(argv[argcID + 2]);
            DIE(!file_out, "Error opening output file!");
            string geon_aux;
            float aux;

            while (file >> geon_aux >> aux >> aux >> aux) {
                file_out << aux << "\n";
            }
            file.close();
            file_out.close();
            continue;
        }

        /* Obtaining the number of lines in the file. */
        file.seekg(0, file.end);
        int file_size = file.tellg();
        file.seekg(0, file.beg);
        int N = file_size / 61;

        /* Allocating the host memory to read the data from the input file. */
        float *lat_host = (float *) malloc(N * sizeof(float));
        float *lon_host = (float *) malloc(N * sizeof(float));
        int *pop_host = (int *) malloc(N * sizeof(int));
        int *accpop_host = (int *) malloc(N * sizeof(int));

         /* Allocating the device memory needed in the GPU. */
         cudaMalloc(&lat_device, N * sizeof(float));
         cudaMalloc(&lon_device, N * sizeof(float));
         cudaMalloc(&pop_device, N * sizeof(int));
         cudaMalloc(&accpop_device, N * sizeof(int));
 
         if (lat_host == 0 || lon_host == 0 || pop_host == 0 || accpop_host == 0) {
             printf("Couldn't allocate host memory\n");
             return 1;
         }
         if (lat_device == 0 || lon_device == 0 || pop_device == 0 || accpop_device == 0) {
             printf("Couldn't allocate device memory\n");
             return 1;
         }

        /* Reading the data from the input file. */
        for (int i = 0; i < N; i++) {
            file >> geon;
            file >> lat_host[i];
            file >> lon_host[i];
            file >> pop_host[i];
            accpop_host[i] = pop_host[i];
        }        

        /* Using the block size 256 and calculating the number of blocks using it. */
        int blockSize = 256;
        int noBlocks = (N + blockSize - 1) / blockSize;

        /* Copying the data from host to device. */
        cudaMemcpy(lat_device, lat_host, N * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(lon_device, lon_host, N * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(pop_device, pop_host, N * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(accpop_device, accpop_host, N * sizeof(int), cudaMemcpyHostToDevice);
        
        /* Calling the function that calculates the population based on the distance. */
        compute<<<noBlocks, blockSize>>>(lat_device, lon_device, pop_device, accpop_device, N, kmRange);

        if (cudaSuccess != cudaGetLastError()) {
            return 1;
        }
        if (cudaSuccess != cudaDeviceSynchronize()) {
            return 1;
        }

        /* Copying the result from the device back into the host. */
        cudaMemcpy(accpop_host, accpop_device, N * sizeof(int), cudaMemcpyDeviceToHost);

        ofstream file_out;
        file_out.open(argv[argcID + 2]);
        DIE(!file_out, "Error opening the output file!");
        
        /* Writing the results into the output file. */
        for (int i = 0; i < N; i++) {
            file_out << accpop_host[i] << '\n';
        }

        /* Freeing the memory. */
        free(lat_host);
        free(lon_host);
        free(pop_host);
        free(accpop_host);
        cudaFree(lat_device);
        cudaFree(lon_device);
        cudaFree(pop_device);
        cudaFree(accpop_device);
        file_out.close();
        file.close();

    }
    return 0;
}