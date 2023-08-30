CC = g++ -Wall -Wextra

all: build

build: server subscriber

# Compileaza server.c
server: server.cpp
	$(CC) $^ -o server

# Compileaza subscriber.c
subscriber: subscriber.cpp
	$(CC) $^ -o subscriber

clean:
	rm -f server subscriber

.PHONY: clean