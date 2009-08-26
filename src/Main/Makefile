TARGET = cria

CRIA = Cria
CRIADIR = ../$(CRIA)
CRIAINC = $(CRIADIR)/include
CRIALIB = lib$(TARGET).so

GC = gc

CC = gcc
CFLAGS = -c -g -DDEBUG -Wall
INCLUDES = -I$(CRIAINC)

OBJS =\
	main.o\
	$(CRIALIB)


#all:main.o
#	cd $(CRIADIR)/source;make;
#	$(CC) -Wl,-rpath,'$$ORIGIN' -o ../../pkg/$(TARGET) main.o -L../../pkg -l$(TARGET) -l$(GC)
all:$(OBJS)
	$(CC) -Wl,-rpath,'$$ORIGIN' -o ../../pkg/$(TARGET) main.o -L../../pkg -l$(TARGET) -l$(GC)

$(CRIALIB):
	cd $(CRIADIR)/source;make;

main.o: main.c
	$(CC) $(CFLAGS) main.c $(INCLUDES)

clean:
	rm -f *.o ../../pkg/$(TARGET)

