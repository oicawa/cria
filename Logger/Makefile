TARGET = Logger.o
CFLAGS = -c -g -DDEBUG -Wall
OBJS =\
    _Logger.o


$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
LoggerTest : $(TARGET) _LoggerTest.o
	$(CC) -o $@ $(TARGET) _LoggerTest.o
.c.o:
	$(CC) $(CFLAGS) -I.. $*.c
clean:
	rm -f *.o LoggerTest



_LoggerTest.o: _LoggerTest.c Logger.h
_Logger.o: _Logger.c _Logger.h Logger.h
