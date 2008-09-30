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
    _FunctionCallStatement.o\
    _StringLiteralExpression.o\
    _ParametersExpression.o\
    _FunctionCallExpression.o\
    _Expression.o\
    _Executor.o\
    _Evaluator.o\
    _IO.o\
    _FunctionDefinition.o\
    _CriaId.o\
    _CriaObject.o\
    _CriaString.o\
#    _Hash.o\
#    _CommentStatement.o\
#    _ReturnStatement.o\
#    _WhileStatement.o\
#    ../Core/Core.o\

$(TARGET):$(OBJS)
	cd ../Logger; make;
	cd ../Memory; make;
	$(CC) -o $@ $(OBJS)

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
_FunctionCallStatement.o: _FunctionCallStatement.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_ExpressionStatement.o: _ExpressionStatement.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_StringLiteralExpression.o: _StringLiteralExpression.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_ParametersExpression.o: _ParametersExpression.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_FunctionCallExpression.o: _FunctionCallExpression.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Expression.o: _Expression.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Executor.o: _Executor.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_Evaluator.o: _Evaluator.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_IO.o: _IO.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_FunctionDefinition.o: _FunctionDefinition.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaId.o: _CriaId.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaObject.o: _CriaObject.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
_CriaString.o: _CriaString.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
#_Hash.o: _Hash.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
#_ReturnStatement.o: _ReturnStatement.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
#_WhileStatement.o: _WhileStatement.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
#_ObjectExpression.o: _ObjectExpression.c _Cria.h Cria.h ../Memory/Memory.h ../Logger/Logger.h
