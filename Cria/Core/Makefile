TARGET = hello.so
CFLAGS = -fPIC -c -g -DDEBUG -Wall
OBJS =\
    hello.o\

all:$(OBJS)
	$(CC) -shared -o $(TARGET) $(OBJS)


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o $(TARGET) $(GCLIB)

hello.o: hello.c
