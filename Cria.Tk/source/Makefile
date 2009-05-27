TARGET = Cria.Tk.so
CFLAGS = -fPIC -c -g -DDEBUG -Wall -I../include -I../../Cria/include -I/usr/include/tcl8.5
CC=gcc

OBJS =\
	Package.o\
	_CriaTk.o\
	_CriaTkControl.o\


all:$(OBJS)
	$(CC) -Wl,-rpath,'$$ORIGIN' -shared -o ../../$(TARGET) $(OBJS) -L../.. -lcria -ltcl8.5 -ltk8.5


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o ../../$(TARGET)

Package.o: Package.c
_CriaTk.o: _CriaTk.c ../include/CriaTk.h
_CriaTkControl.o: _CriaTkControl.c ../include/CriaTkControl.h

