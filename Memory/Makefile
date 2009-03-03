TARGET = Memory.o
CFLAGS = -c -g -DDEBUG -Wall -I../Library/include/
LDFLAGS = -L../Library/lib -lgc
CC=gcc
OBJS =\
    _Memory.o


$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS) $(LDFLAGS)
MemoryTest : $(TARGET) _MemoryTest.o
	$(CC) -o $@ $(TARGET) _MemoryTest.o $(LDFLAGS)
.c.o:
	$(CC) $(CFLAGS) -I.. $*.c
clean:
	rm -f *.o MemoryTest



_MemoryTest.o: _MemoryTest.c Memory.h
_Memory.o: _Memory.c _Memory.h Memory.h
