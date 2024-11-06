CC = gcc
CFLAGS = -Wall -g

all: echoserver

echoserver: main.o server.o
	$(CC) $(CFLAGS) -o echoserver main.o server.o

main.o: main.c server.h
	$(CC) $(CFLAGS) -c main.c

server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f *.o echoserver
