#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "genetic_algorithm.h"

int read_input(sack_object **objects, int *object_count, int *sack_capacity, int *generations_count, int *P, double *object_countM, int argc, char *argv[]) {
	FILE *fp;

	if (argc < 4) {
		fprintf(stderr, "Usage:\n\t./tema1 in_file generations_count P\n");
		return 0;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		return 0;
	}

	if (fscanf(fp, "%d %d", object_count, sack_capacity) < 2) {
		fclose(fp);
		return 0;
	}
	
	// calculating the object count used in the merge sort
	if (ceil((double) log2(*object_count)) == floor((double) log2(*object_count))) {
		(*object_countM) = (*object_count);
	} else {
		(*object_countM) = pow(2, (int) log2(*object_count) + 1);
	}

	if (*object_count % 10) {
		fclose(fp);
		return 0;
	}

	sack_object *tmp_objects = (sack_object *) calloc(*object_count, sizeof(sack_object));

	for (int i = 0; i < *object_count; ++i) {
		if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) < 2) {
			free(objects);
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);

	*generations_count = (int) strtol(argv[2], NULL, 10);
	
	if (*generations_count == 0) {
		free(tmp_objects);

		return 0;
	}

	*objects = tmp_objects;

	*P = (int) strtol(argv[3], NULL, 10);

	return 1;
}

void print_best_fitness(const individual *generation) {
	printf("%d\n", generation[0].fitness);
}

void compute_fitness_function(const sack_object *objects, individual **generation, int object_count, int sack_capacity, int start, int end) {
	int weight;
	int profit;
	for (int i = start; i < end; ++i) {
			weight = 0;
			profit = 0;

			for (int j = 0; j < (*generation)[i].chromosome_length; ++j) {
				if ((*generation)[i].chromosomes[j]) {
					weight += objects[j].weight;
					profit += objects[j].profit;
				}
			}

			(*generation)[i].fitness = (weight <= sack_capacity) ? profit : 0;
		}
}

int cmpfunc(const void *a, const void *b) {
	int i;
	individual *first = (individual *) a;
	individual *second = (individual *) b;
	
	int res = second->fitness - first->fitness; // decreasing by fitness
	if (res == 0) {
		int first_count = 0, second_count = 0;

		for (i = 0; i < first->chromosome_length && i < second->chromosome_length; ++i) {
			first_count += first->chromosomes[i];
			second_count += second->chromosomes[i];
		}

		res = first_count - second_count; // increasing by number of objects in the sack
		if (res == 0) {
			return second->index - first->index;
		}
	}

	return res;
}

void merge(individual **source, int start, int mid, int end, individual **destination) {
	int iA = start;
	int iB = mid;
	int i;

	for (i = start; i < end; i++) {
		if (end == iB || (iA < mid && cmpfunc(&(*source)[iA], &(*source)[iB]) < 0)) {
			(*destination)[i] = (*source)[iA];
			iA++;
		} else {
			(*destination)[i] = (*source)[iB];
			iB++;
		}
	}

}

void mutate_bit_string_1(const individual *ind, int generation_index) {
	int i, mutation_size;
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	if (ind->index % 2 == 0) {
		// for even-indexed individuals, mutate the first 40% chromosomes by a given step
		mutation_size = ind->chromosome_length * 4 / 10;
		for (i = 0; i < mutation_size; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	} else {
		// for even-indexed individuals, mutate the last 80% chromosomes by a given step
		mutation_size = ind->chromosome_length * 8 / 10;
		for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	}
}

void mutate_bit_string_2(const individual *ind, int generation_index) {
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	// mutate all chromosomes by a given step
	for (int i = 0; i < ind->chromosome_length; i += step) {
		ind->chromosomes[i] = 1 - ind->chromosomes[i];
	}
}

void crossover(individual *parent1, individual *child1, int generation_index) {
	individual *parent2 = parent1 + 1;
	individual *child2 = child1 + 1;
	int count = 1 + generation_index % parent1->chromosome_length;

	memcpy(child1->chromosomes, parent1->chromosomes, count * sizeof(int));
	memcpy(child1->chromosomes + count, parent2->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));

	memcpy(child2->chromosomes, parent2->chromosomes, count * sizeof(int));
	memcpy(child2->chromosomes + count, parent1->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));
}

void copy_individual(const individual *from, const individual *to) {
	memcpy(to->chromosomes, from->chromosomes, from->chromosome_length * sizeof(int));
}

void free_generation(individual *generation){
	int i;

	for (i = 0; i < generation->chromosome_length; ++i) {
		free(generation[i].chromosomes);
		generation[i].chromosomes = NULL;
		generation[i].fitness = 0;
	}
}

void *run_genetic_algorithm(void *arg)
{
	int r;
	struct my_arg *data = (struct my_arg*) arg;
	int width;
	
	int count, cursor;
	individual **current_generation = data->current_generation;
	individual **next_generation = data->next_generation;
	individual **some_generation = data->some_generation;
	individual *tmp = NULL;

	// calculating the starting and ending points for the arrays
	int start = data->thread_id * (double) data->object_count / data->P;
	int end = min((data->thread_id + 1) * (double) data->object_count / data->P, data->object_count);

	// set initial generation (composed of object_count individuals with a single item in the sack)
	for (int i = start; i < end; ++i) {
		(*current_generation)[i].fitness = 0;
		(*current_generation)[i].chromosomes = (int*) calloc(data->object_count, sizeof(int));
		(*current_generation)[i].chromosomes[i] = 1;
		(*current_generation)[i].index = i;
		(*current_generation)[i].chromosome_length = data->object_count;

		(*next_generation)[i].fitness = 0;
		(*next_generation)[i].chromosomes = (int*) calloc(data->object_count, sizeof(int));
		(*next_generation)[i].index = i;
		(*next_generation)[i].chromosome_length = data->object_count;

	}

	r = pthread_barrier_wait(data->barrier);
	if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
		printf("Eroare la asteptare\n");
		exit(-1);
	}

	// iterate for each generation
	for (int k = 0; k < data->generations_count; ++k) {
		cursor = 0;

		compute_fitness_function(data->objects, current_generation, data->object_count, data->sack_capacity, start, end);
		
		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}

		// compute fitness and sort by it

		for (width = 1; width < data->object_countM; width = 2 * width) {
			for (int i = 2 * data->thread_id * width; i < data->object_countM; i = i + 2 * width * data->P) {
				merge(current_generation, i, i + width, i + 2 * width, some_generation);
			}
			
			r = pthread_barrier_wait(data->barrier);
			if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
				printf("Eroare la asteptare\n");
				exit(-1);
			}
			
			// swapping the current_generation with the sorted one.
			if (data->thread_id == 0) {
				tmp = *(current_generation);
				*(current_generation) = *(some_generation);
				*(some_generation) = tmp;
			}

			r = pthread_barrier_wait(data->barrier);
			if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
				printf("Eroare la asteptare\n");
				exit(-1);
			}
		}

		// keep first 30% children (elite children selection)
		count = data->object_count * 3 / 10;
		start =  data->thread_id * (double) count / data->P;
		end = min((data->thread_id + 1) * (double) count / data->P, count);
		for (int i = start; i < end; ++i) {
			copy_individual((*current_generation) + i, (*next_generation) + i);
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}
		
		cursor = count;

		// mutate first 20% children with the first version of bit string mutation
		count = data->object_count * 2 / 10;
		start =  data->thread_id * (double) count / data->P;
		end = min((data->thread_id + 1) * (double) count / data->P, count);
		for (int i = start; i < end; ++i) {
			copy_individual((*current_generation) + i, (*next_generation) + cursor + i);
			mutate_bit_string_1((*next_generation) + cursor + i, k);
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}

		cursor += count;

		// mutate next 20% children with the second version of bit string mutation
		count = data->object_count * 2 / 10;
		start =  data->thread_id * (double) count / data->P;
		end = min((data->thread_id + 1) * (double) count / data->P, count);
		for (int i = start; i < end; ++i) {
			copy_individual((*current_generation) + i + count, (*next_generation) + cursor + i);
			mutate_bit_string_2((*next_generation) + cursor + i, k);
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}

		cursor += count;

		// crossover first 30% parents with one-point crossover
		// (if there is an odd number of parents, the last one is kept as such)
		count = data->object_count * 3 / 10;
		start =  data->thread_id * (double) count / data->P;
		end = min((data->thread_id + 1) * (double) count / data->P, count);
		if (count % 2 == 1) {
			copy_individual((*current_generation) + data->object_count - 1, (*next_generation) + cursor + count - 1);
			count--;
		}

		for (int i = start; i < end - 1; i += 2) {
			crossover((*current_generation) + i, (*next_generation) + cursor + i, k);
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}

		// switch to new generation only on one thread
		if (data->thread_id == 0) {
			tmp = (*current_generation);
			(*current_generation) = (*next_generation);
			(*next_generation) = tmp;
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}

		start = data->thread_id * (double) data->object_count / data->P;
		end = min((data->thread_id + 1) * (double) data->object_count / data->P, data->object_count);
		for (int i = start; i < end - 1; ++i) {
			(*current_generation)[i].index = i;
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}

		// printing the best fitness only once
		if (data->thread_id == 0) {
			if (k % 5 == 0) {
				print_best_fitness((*current_generation));
			}
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}
	}

	compute_fitness_function(data->objects, current_generation, data->object_count, data->sack_capacity, start, end);

	r = pthread_barrier_wait(data->barrier);
	if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
		printf("Eroare la asteptare\n");
		exit(-1);
	}


	for (width = 1; width < data->object_countM; width = 2 * width) {
		for (int i = 2 * data->thread_id * width; i < data->object_countM; i = i + 2 * width * data->P) {
			merge(current_generation, i, i + width, i + 2 * width, some_generation);
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}

		// swapping the current_generation with the sorted one
		if (data->thread_id == 0) {
			tmp = *(current_generation);
			*(current_generation) = *(some_generation);
			*(some_generation) = tmp;
		}

		r = pthread_barrier_wait(data->barrier);
		if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
			printf("Eroare la asteptare\n");
			exit(-1);
		}
	}
	// freeing the memory only on one thread
	if (data->thread_id == 0) {
		print_best_fitness((*current_generation));

		// free resources for old generation
		free_generation((*current_generation));
		free_generation((*next_generation));
		// free resources
		free((*current_generation));
		free((*next_generation));
		free((*some_generation));
	}
	r = pthread_barrier_wait(data->barrier);
	if(r && r != PTHREAD_BARRIER_SERIAL_THREAD) {
		printf("Eroare la asteptare\n");
		exit(-1);
	}
	pthread_exit(NULL);

}

int main(int argc, char *argv[]) {
	int r;
	void *status;

	// the threads
	pthread_t *threads;

	//the barrier
	pthread_barrier_t barrier;

	// our structure that's gonna be passed to the function of the threads.
	struct my_arg *arguments;

	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;

	// number of objects
	int object_count = 0;

	// maximum weight that can be carried in the sack
	int sack_capacity = 0;

	// number of generations
	int generations_count = 0;

	// number of threads
	int P = 0;

	// number of objects used in the merge sort.
	double object_countM = 0;

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, &P, &object_countM, argc, argv)) {
		return 0;
	}

	arguments = (struct my_arg*) malloc(P * sizeof(struct my_arg));
	threads = (pthread_t*) malloc(P * sizeof(pthread_t));
	individual *current_generation = (individual*) calloc(object_countM, sizeof(individual));
	individual *next_generation = (individual*) calloc(object_countM, sizeof(individual));
	individual *some_generation = (individual*) calloc(object_countM, sizeof(individual));
	

	pthread_barrier_init(&barrier, NULL, P);

	for (int i = 0; i < P; i++) {
		arguments[i].thread_id = i;
		arguments[i].objects = objects;
		arguments[i].object_count = object_count;
		arguments[i].sack_capacity = sack_capacity;
		arguments[i].generations_count = generations_count;
		arguments[i].current_generation = &current_generation;
		arguments[i].some_generation = &some_generation;
		arguments[i].next_generation = &next_generation;
		arguments[i].barrier = &barrier;
		arguments[i].P = P;
		arguments[i].object_countM = object_countM;

		r = pthread_create(&threads[i], NULL, run_genetic_algorithm, &arguments[i]);
		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (int i = 0; i < P; i++) {
		r = pthread_join(threads[i], &status);
		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	free(objects);
	free(arguments);
	free(threads);
	pthread_barrier_destroy(&barrier);
	return 0;
}