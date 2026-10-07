CC = gcc
CFLAGS = -O0 -Wall

all: graph

graph: source\ code.c
	$(CC) $(CFLAGS) -o graph "source code.c"

run: graph
	./graph

clean:
	rm -f graph

.PHONY: all run clean
