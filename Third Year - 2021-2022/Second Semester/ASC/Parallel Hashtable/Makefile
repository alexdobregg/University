all: gpu_hashtable

gpu_hashtable: src/*.cu
	nvcc -arch=sm_35 -O2 -g -std=c++11 -I. -I./src *.cu ./src/*.cu -o gpu_hashtable

run:
	python3 bench.py

clean:
	rm -rf gpu_hashtable
	rm -rf slurm-*
