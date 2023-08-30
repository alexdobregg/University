CC = g++
CFLAGS = -Wall -Wextra -O2
LDLIBS = -lm

build: algo_test.o

algo.o: kmp.cpp boyermoore.cpp
	g++ kmp.cpp -c $(CFLAGS)
	g++ boyermoore.cpp -c $(CFLAGS)

algo_test.o: algo_test.cpp algo.o
	g++ algo_test.cpp -o test $(CFLAGS)

run_tests:
	./test
.PHONY: clean
clean:
	rm -rf *.o test
