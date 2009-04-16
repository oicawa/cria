TARGET = Cria.Tk.so
CFLAGS = -fPIC -c -g -DDEBUG -Wall -I../include -I../../Cria/include
CC=gcc

OBJS =\
	Package.o\
	_TkWindow.o\


all:$(OBJS)
	$(CC) -Wl,-rpath,'$$ORIGIN' -shared -o ../../$(TARGET) $(OBJS) -L../.. -lcria


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o ../../$(TARGET)

Package.o: Package.c
_Window.o: _TkWindow.c ../include/TkWindow.h

