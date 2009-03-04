TARGET = cria

CC=gcc
CFLAGS = -c -g -DDEBUG -Wall
INCLUDE_CRIA = -I../Cria
INCLUDE_GC = -I../Library/include
LD_CRIA = -L. -lcria
LD_GC = -L../Library/lib -lgc

all:main.o
	$(CC) -o cria main.o ../Cria/libcria.so ../Library/lib/libgc.so

main.o: main.c
	$(CC) $(CFLAGS) main.c $(INCLUDE_CRIA)

clean:
	rm -f *.o cria

