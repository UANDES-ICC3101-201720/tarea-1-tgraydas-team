CC=gcc
CFLAGS=-Wall -std=c11 -Werror

all: binsearch datagen

datagen: datagen.c 
	$(CC) -o datagen datagen.c -lm $(CFLAGS)

binsearch: binsearch.c util.o
	$(CC) -o binsearch binsearch.c util.o -lm -pthread $(CFLAGS)

util.o:
	$(CC) -c -o util.o util.c $(CFLAGS)

clean:
	rm datagen binsearch *.o
 
