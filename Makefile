TARGET = cria

CRIA = $(TARGET)
CRIADIR = Cria
CRIAINC = Cria
CRIALIB = lib$(CRIA).so

GC = gc
GCLIB = lib$(GC).so
GCINC = Library/include

CC = gcc
CFLAGS = -c -g -DDEBUG -Wall
INCLUDES = -I$(CRIAINC) -I$(GCINC)


all:main.o
	cp Library/lib/$(GCLIB) ./$(GCLIB);
	cp Cria/$(CRIALIB) ./$(CRIALIB);
	$(CC) -Wl,-rpath,'$$ORIGIN/$(CRIADIR)' -o cria main.o -L$(CRIADIR) -l$(CRIA) -l$(GC)

main.o: main.c
	$(CC) $(CFLAGS) main.c $(INCLUDES)

clean:
	rm -f *.o $(TARGET) $(CRIALIB) $(GCLIB)

