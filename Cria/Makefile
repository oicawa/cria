TARGET = cria
CFLAGS = -c -g -DDEBUG -Wall
OBJS =\
    main.o\
    ../Memory/Memory.o\
    ../Logger/Logger.o\
    _String.o\
    _List.o\
    _StringBuffer.o\
    _Interpreter.o\
    _DefinitionVariable.o\
    _DefinitionFunction.o\
    _DefinitionClass.o\
    _CriaFile.o\
    _Tokenizer.o\
    _Parser.o\
    _Statement.o\
    _Expression.o\
    _IO.o\
    _CriaId.o\
    _CriaString.o\
    _CriaInteger.o\
    _CriaBoolean.o\
    _Reference.o\

all:$(OBJS)
	cd ../Logger; make;
	cd ../Memory; make;
	$(CC) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -I.. $*.c

clean:
	rm -f *.o cria

main.o: main.c
_String.o: _String.c _String.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_List.o: _List.c _List.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_StringBuffer.o: _StringBuffer.c _StringBuffer.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Interpreter.o: _Interpreter.c _Interpreter.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaId.o: _CriaId.c _CriaId.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaString.o: _CriaString.c _CriaString.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaInteger.o: _CriaInteger.c _CriaInteger.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaBoolean.o: _CriaBoolean.c _CriaBoolean.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaFile.o: _CriaFile.c _CriaFile.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_DefinitionVariable.o: _DefinitionVariable.c _DefinitionVariable.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_DefinitionFunction.o: _DefinitionFunction.c _DefinitionFunction.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_DefinitionClass.o: _DefinitionClass.c _DefinitionClass.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Tokenizer.o: _Tokenizer.c _Tokenizer.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Parser.o: _Parser.c _Parser.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Statement.o: _Statement.c _Statement.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Expression.o: _Expression.c _Expression.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_IO.o: _IO.c _IO.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Reference.o: _Reference.c _Reference.h _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
