.phony: build clean

CC = gcc
CFLAGS = -Wall -std=c99
DEPS = $(wildcard *.h)
OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
EXEC = movies

build: $(EXEC)

$(EXEC):$(OBJ)
	gcc $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJ)
