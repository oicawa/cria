TARGET = libcria.so
CFLAGS = -c -g -DDEBUG -Wall -I../Library/include/
LDFLAGS = -L../Library/lib/ -lgc
OBJS =\
    ../Memory/Memory.o\
    ../Logger/Logger.o\
    _String.o\
    _List.o\
    _StringBuffer.o\
    _Interpreter.o\
    _Definition.o\
    _Tokenizer.o\
    _Parser.o\
    _Statement.o\
    _Expression.o\
    _Reference.o\
    _Hash.o\
    _IO.o\
    _CriaId.o\
    _CriaString.o\
    _CriaInteger.o\
    _CriaBoolean.o\
    _CriaFile.o\
    _CriaList.o\
    _CriaObject.o\
    _CriaClass.o\

all:$(OBJS)
	cd ../Logger; make;
	cd ../Memory; make;
	$(CC) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -I.. $*.c

clean:
	rm -f *.o cria

_String.o: _String.c _String.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_List.o: _List.c _List.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_StringBuffer.o: _StringBuffer.c _StringBuffer.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Interpreter.o: _Interpreter.c _Interpreter.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaId.o: _CriaId.c _CriaId.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaString.o: _CriaString.c _CriaString.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaInteger.o: _CriaInteger.c _CriaInteger.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaBoolean.o: _CriaBoolean.c _CriaBoolean.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaFile.o: _CriaFile.c _CriaFile.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaList.o: _CriaList.c _CriaList.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaObject.o: _CriaObject.c _CriaObject.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaClass.o: _CriaClass.c _CriaClass.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Definition.o: _Definition.c _Definition.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Tokenizer.o: _Tokenizer.c _Tokenizer.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Parser.o: _Parser.c _Parser.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Statement.o: _Statement.c _Statement.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Expression.o: _Expression.c _Expression.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_IO.o: _IO.c _IO.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Reference.o: _Reference.c _Reference.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Hash.o: _Hash.c _Hash.h Hash.h
