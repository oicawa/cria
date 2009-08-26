TARGET = cria

CRIA = Cria
CRIADIR = ../$(CRIA)
CRIAINC = $(CRIADIR)/include
CRIALIB = lib$(TARGET).so

GC = gc

CC = gcc
CFLAGS = -c -g -DDEBUG -Wall
INCLUDES = -I$(CRIAINC)


all:main.o
	cd $(CRIADIR)/source;make;
	$(CC) -Wl,-rpath,'$$ORIGIN' -o ../$(TARGET) main.o -L.. -l$(TARGET) -l$(GC)

main.o: main.c
	$(CC) $(CFLAGS) main.c $(INCLUDES)

clean:
	rm -f *.o ../$(TARGET)

