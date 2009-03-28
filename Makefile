TARGET = cria

CRIA = $(TARGET)
CRIAINC = ../Cria
CRIALIB = lib$(CRIA).so

GC = gc
GCLIB = lib$(GC).so
GCINC = ../Library/include

CC = gcc
CFLAGS = -c -g -DDEBUG -Wall
INCLUDES = -I$(CRIAINC) -I$(GCINC)


all:main.o
	cp ../Library/lib/$(GCLIB) ./$(GCLIB);
	cp ../Cria/$(CRIALIB) ./$(CRIALIB);
	$(CC) -Wl,-rpath,. -o cria main.o -L. -l$(CRIA) -l$(GC)
	#$(CC) -Wl,-rpath,'$ORIGIN' -o cria main.o -L. -l$(CRIA) -l$(GC)
	#ld -o cria main.o -L. -l$(CRIA) -l$(GC)

main.o: main.c
	$(CC) $(CFLAGS) main.c $(INCLUDES)

clean:
	rm -f *.o $(TARGET) $(CRIALIB) $(GCLIB)

