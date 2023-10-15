CC=gcc
CFLAGS=-Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2

all: labirynth
.PHONY: clean all

error.o: error.h
queue.o: queue.h error.h
input.o: input.h error.h
correct.o: correct.h error.o input.o

labirynth: labirynth.o input.o queue.o error.o correct.o
	$(CC) $(CFLAGS) -o labirynth $^

clean:
	-rm -f *.o labirynth
