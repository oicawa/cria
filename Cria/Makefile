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
    _Tokenizer.o\
    _Parser.o\
    _Statement.o\
    _Expression.o\
    _Executor.o\
    _Evaluator.o\
    _IO.o\
    _DefinitionVariable.o\
    _DefinitionFunction.o\
    _DefinitionClass.o\
    _CriaId.o\
    _CriaObject.o\
    _CriaString.o\
    _CriaInteger.o\
    _CriaBoolean.o\
    _CriaFile.o\
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
_String.o: _String.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_List.o: _List.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_StringBuffer.o: _StringBuffer.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Interpreter.o: _Interpreter.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Tokenizer.o: _Tokenizer.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Parser.o: _Parser.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Statement.o: _Statement.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Expression.o: _Expression.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Executor.o: _Executor.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Evaluator.o: _Evaluator.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_IO.o: _IO.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_DefinitionVariable.o: _DefinitionVariable.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_DefinitionFunction.o: _DefinitionFunction.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_DefinitionClass.o: _DefinitionClass.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaId.o: _CriaId.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaObject.o: _CriaObject.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaString.o: _CriaString.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaInteger.o: _CriaInteger.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaBoolean.o: _CriaBoolean.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaFile.o: _CriaFile.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Reference.o: _Reference.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h

