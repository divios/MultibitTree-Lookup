CC=gcc

all: main.c utils.c io.c
	$(CC) -o main main.c utils.c io.c -lm

.PHONY: clean
clean:
	rm -f *.o main