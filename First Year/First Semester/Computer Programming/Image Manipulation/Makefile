CC = gcc
CFLAG = -Wall -Wextra
EXE = bmp
OBJ = bmp.o

build: $(EXE)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAG)

$(EXE): $(OBJ)
	$(CC) $^ $(CFLAG) -o $(EXE) 

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -f *.o $(EXE)
