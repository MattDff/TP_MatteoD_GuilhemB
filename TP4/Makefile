CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L. -lseg

all: client

client: client.o
	$(CC) $(CFLAGS) -o client client.o $(LDFLAGS)

client.o: client.c segdef.h
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f client client.o
