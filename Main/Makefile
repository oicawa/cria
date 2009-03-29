TARGET = cria

LIBDIR = ../Library/lib

CRIA = Cria
CRIADIR = ../$(CRIA)
CRIAINC = $(CRIADIR)/include
CRIALIB = lib$(TARGET).so

GC = gc
GCLIB = lib$(GC).so
GCINC = Library/include

CC = gcc
CFLAGS = -c -g -DDEBUG -Wall
INCLUDES = -I$(CRIAINC) -I$(GCINC)


all:main.o $(CRIALIB)
	cd $(CRIADIR)/source;make;
	$(CC) -Wl,-rpath,'$$ORIGIN' -o ../$(TARGET) main.o -L.. -l$(TARGET) -L$(LIBDIR) -l$(GC)

main.o: main.c
	$(CC) $(CFLAGS) main.c $(INCLUDES)

$(CRIALIB): 
	
	
clean:
	rm -f *.o ../$(TARGET)

