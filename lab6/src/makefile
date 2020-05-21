CC=gcc
CFLAGS=-I.

all: server client

server: libs server.c
	$(CC) server.c -L./ -llib -lpthread -o server.out $(CFLAGS)

client: libs client.c
	$(CC) client.c -L./ -llib -lpthread -o client.out $(CFLAGS)

libs: lib.c
	$(CC) -c lib.c -o lib.out
	ar rcs liblib.a lib.out

clean :
	rm client.out server.out