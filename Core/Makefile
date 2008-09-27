TARGET = Core.o
CFLAGS = -c -g -DDEBUG -Wall
OBJS =\
    _IO.o\
    ../Memory/Memory.o\
    ../Logger/Logger.o

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
CoreTest : $(TARGET) _CoreTest.o
	$(CC) -o $@ $(TARGET) _CoreTest.o
.c.o:
	$(CC) $(CFLAGS) -I.. $*.c
clean:
	rm -f *.o CoreTest
../Memory/Memory.o:
	cd ../Memory; make;
../Logger/Logger.o:
	cd ../Logger; make;

_CoreTest.o: _CoreTest.c Core.h ../Memory/Memory.h ../Logger/Logger.h
_IO.o: _IO.c _Core.h Core.h ../Memory/Memory.h ../Logger/Logger.h
