CC = g++
FLAGS = -std=c++11



build: client.cpp requests.cpp helpers.cpp buffer.cpp
	$(CC) $(FLAGS) $^ -o client

run:
	./client

clean:
	rm -f *.o client

.PHONY: clean