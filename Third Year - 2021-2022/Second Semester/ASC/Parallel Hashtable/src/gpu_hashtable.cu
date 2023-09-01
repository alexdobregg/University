#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <sstream>
#include <string>
#include "test_map.hpp"
#include "gpu_hashtable.hpp"

using namespace std;

// Function used by murmur_hash
__device__ uint32_t murmur_scramble(uint32_t k) {
	k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

/* Function to calculate the hash based on the key using the murmur hash
algorithm, disregarding the len parameter. Also seed is set to a random number
between 2000000000 & 3000000000. Given the fact that rand() is a host function,
a computation would be necessary everytime before calling this function. Therefore,
this is why the seed is hardcoded to 2100682596*/
__device__ uint32_t murmur_hash(int key) {
    uint32_t h = 2100682596;
    uint32_t k = key;
    h ^= murmur_scramble(k);
    h = (h << 13) | (h >> 19);
    h = h * 5 + 0xe6546b64;
    h ^= murmur_scramble(k);
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h; 
}


/* The kernel function executed on the GPU to realloc the values from the curr_entries variable
to the new_entries variable which has a bigger capacity. It is based on the linear probing.*/
__global__ void kernel_reshape(Entry *new_entries, int new_max_size, Entry *curr_entries, int curr_max_size) {
	int idx = threadIdx.x + blockIdx.x * blockDim.x ;
	if (idx < curr_max_size && curr_entries[idx].key != 0) {
		int key = curr_entries[idx].key;
		int value = curr_entries[idx].value;
		uint32_t hash = murmur_hash(key) % new_max_size;

		while (atomicCAS(&new_entries[hash].key, 0, key) != 0) {
			hash = (hash + 1) % new_max_size;
		}
		new_entries[hash].value = value;
	}
}

/* The kernel function executed on the GPU to insert a new value into the entries array, also based on the linear
probing. If the key already exists, update its value and subtract from the size, otherwise insert a new pair of key-value.*/
__global__ void kernel_insert(Entry *entries, int numKeys, int max_size, int *keys, int *values, int *updated_keys) {
	int idx = threadIdx.x + blockIdx.x * blockDim.x ;
	if (idx < numKeys) {
		int key = keys[idx];
		int value = values[idx];
		uint32_t hash = murmur_hash(key) % max_size;
		int old_key = atomicCAS(&entries[hash].key, 0, key);

		while (old_key != 0 && old_key != key) {
			hash = (hash + 1) % max_size;
			old_key = atomicCAS(&entries[hash].key, 0, key);
		}
		entries[hash].value = value;

		if (old_key == key)
			atomicAdd(updated_keys, 1);
	}
}

/*The kernel function executed on GPU to extract the values and save them in the values variable.
It also searches based on the linear probing*/
__global__ void kernel_get(Entry *entries, int numKeys, int max_size, int *keys, int *values) {
	int idx = threadIdx.x + blockIdx.x * blockDim.x ;
	if (idx < numKeys) {
		int key = keys[idx];
		uint32_t hash = murmur_hash(key) % max_size;

		while (key != entries[hash].key) {
			hash = (hash + 1) % max_size;
		}
		values[idx] = entries[hash].value;
	}
}

/*
Allocate CUDA memory only through glbGpuAllocator
cudaMalloc -> glbGpuAllocator->_cudaMalloc
cudaMallocManaged -> glbGpuAllocator->_cudaMallocManaged
cudaFree -> glbGpuAllocator->_cudaFree
*/

/**
 * Function constructor GpuHashTable
 * Performs init
 * Example on using wrapper allocators _cudaMalloc and _cudaFree
 */
GpuHashTable::GpuHashTable(int size) {
	hashtable.curr_size = 0;
	hashtable.max_size = size;

	glbGpuAllocator->_cudaMalloc((void **)&hashtable.entries, size * sizeof(Entry));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't allocate memory for the hashtable entries!");
		exit(0);
	}

	cudaMemset(hashtable.entries, 0, size * sizeof(Entry));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't set the values of entries to 0!");
		exit(0);
	}
}

/**
 * Function desctructor GpuHashTable
 */
GpuHashTable::~GpuHashTable() {
	glbGpuAllocator->_cudaFree(hashtable.entries);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't free the hashtable entries");
		exit(0);
	}
}

/**
 * Function reshape
 * Performs resize of the hashtable based on load factor
 */
void GpuHashTable::reshape(int numBucketsReshape) {
	Entry *new_entries;
	int block_size = 256;
    int blocks_number = (hashtable.max_size + block_size - 1) / block_size;

	glbGpuAllocator->_cudaMalloc((void **)&new_entries, numBucketsReshape * sizeof(Entry));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't allocate memory for the new entries!");
		exit(0);
	}

	cudaMemset(new_entries, 0, numBucketsReshape * sizeof(Entry));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't set the values of the new entries to 0!");
		exit(0);
	}

	kernel_reshape<<<blocks_number, block_size>>>(new_entries, numBucketsReshape, hashtable.entries, hashtable.max_size);
	if (cudaSuccess != cudaDeviceSynchronize()) {
		printf("Couldn't synchronize after kernel reshape!");
		exit(0);
	}

	glbGpuAllocator->_cudaFree(hashtable.entries);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't free the memory for the current hashtable entries");
		exit(0);
	}

	hashtable.entries = new_entries;
	hashtable.max_size = numBucketsReshape;
}

/**
 * Function insertBatch
 * Inserts a batch of key:value, using GPU and wrapper allocators
 */
bool GpuHashTable::insertBatch(int *keys, int* values, int numKeys) {
	int *keys_device;
	int *values_device;
	int *updated_keys;
	int block_size = 256;
    int blocks_number = (numKeys + block_size - 1) / block_size;

	glbGpuAllocator->_cudaMalloc((void **)&keys_device, numKeys * sizeof(int));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't allocate memory for device keys!");
		exit(0);
	}

	glbGpuAllocator->_cudaMalloc((void **)&values_device, numKeys * sizeof(int));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't allocate memory for device values!");
		exit(0);
	}

	glbGpuAllocator->_cudaMallocManaged((void **)&updated_keys, sizeof(int));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't allocate memory for updated_keys variable!");
		exit(0);
	}
	*updated_keys = 0;

	cudaMemcpy(keys_device, keys, numKeys * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't copy the host keys over device keys!");
		exit(0);
	}

	cudaMemcpy(values_device, values, numKeys * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't copy the host values over device values!");
		exit(0);
	}

	if ((hashtable.curr_size + numKeys) / hashtable.max_size >= LOAD_FACTOR_MAX)
		reshape((hashtable.curr_size + numKeys) / LOAD_FACTOR_MIN);
	

	kernel_insert<<<blocks_number, block_size>>>(hashtable.entries, numKeys, hashtable.max_size, keys_device, values_device, updated_keys);
	if (cudaSuccess != cudaDeviceSynchronize()) {
		printf("Couldn't synchronize after kernel insert!");
		exit(0);
	}

	hashtable.curr_size = hashtable.curr_size + numKeys - *updated_keys;

	glbGpuAllocator->_cudaFree(keys_device);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't free the device keys!");
		exit(0);
	}
	
	glbGpuAllocator->_cudaFree(values_device);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't free the device values!");
		exit(0);
	}

	glbGpuAllocator->_cudaFree(updated_keys);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't free the updated_keys variable!");
		exit(0);
	}

	return true;
}

/**
 * Function getBatch
 * Gets a batch of key:value, using GPU
 */
int* GpuHashTable::getBatch(int* keys, int numKeys) {
	int *values_device;
	int *keys_device;
	
	int block_size = 256;
    int blocks_number = (numKeys + block_size - 1) / block_size;

	glbGpuAllocator->_cudaMalloc((void **)&keys_device, numKeys * sizeof(int));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't allocate memory for the device keys!");
		exit(0);
	}

	cudaMemcpy(keys_device, keys, numKeys * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't copy the host keys over device keys!");
		exit(0);
	}

	glbGpuAllocator->_cudaMalloc((void **)&values_device, numKeys * sizeof(int));
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't allocate memory for the device values!");
		exit(0);
	}

	kernel_get<<<blocks_number, block_size>>>(hashtable.entries, numKeys, hashtable.max_size, keys_device, values_device);
	if (cudaSuccess != cudaDeviceSynchronize()) {
		printf("Couldn't synchronize after kernel get!");
		exit(0);
	}

	int *values_host = (int *) malloc(numKeys * sizeof(int));
	if (!values_host) {
		printf("Couldn't allocate memory for host values");
		exit(0);
	}

	cudaMemcpy(values_host, values_device, numKeys * sizeof(int), cudaMemcpyDeviceToHost);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't copy the device values over host values!");
		exit(0);
	}

	glbGpuAllocator->_cudaFree(keys_device);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't free the device keys!");
		exit(0);
	}

	glbGpuAllocator->_cudaFree(values_device);
	if (cudaSuccess != cudaGetLastError()) {
		printf("Couldn't free the device values!");
		exit(0);
	}

	return values_host;
}
