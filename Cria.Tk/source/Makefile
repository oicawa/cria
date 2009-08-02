TARGET = Cria.Tk.so
CFLAGS = -fPIC -c -g -DDEBUG -Wall -I../include -I../../Cria/include -I/usr/include/tcl8.5
CC=gcc

OBJS =\
	Package.o\
	CriaTk.o\


all:$(OBJS)
	$(CC) -Wl,-rpath,'$$ORIGIN' -shared -o ../../$(TARGET) $(OBJS) -L../.. -lcria -ltcl8.5 -ltk8.5


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o ../../$(TARGET)

Package.o: Package.c
CriaTk.o: CriaTk.c CriaTk.h

