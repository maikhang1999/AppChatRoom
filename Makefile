CFLAGS = -c -Wall
CC = gcc
LIBS=  -pthread

all: client server

debug: CFLAGS += -g
debug: client server

client: client.o user.o
	${CC} client.o user.o -o client $(LIBS)

server: server.o user.o word.o
	${CC} server.o user.o word.o -o server $(LIBS)

client.o: client.c
	${CC} ${CFLAGS} client.c

server.o: server.c
	${CC} ${CFLAGS}  server.c
user.o: user.c
	${CC} ${CFLAGS}  user.c
word.o: word.c
	${CC} ${CFLAGS} word.c
clean:
	rm -f *.o *~
