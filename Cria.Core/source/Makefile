TARGET = Cria.Core.so
CFLAGS = -fPIC -c -g -DDEBUG -Wall -I../include
CC=gcc

OBJS =\
    Package.o\

all:$(OBJS)
	$(CC) -shared -o ../../$(TARGET) $(OBJS)


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o ../../$(TARGET)

Package.o: Package.c
