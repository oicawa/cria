TARGET = libcria.so
CFLAGS = -fPIC -c -g -DDEBUG -Wall -I../../Library/include/ -I../include
LDFLAGS = -L../../Library/lib -lgc
GC = gc
GCLIB = lib$(GC).so
OBJS =\
	_Memory.o\
	_Logger.o\
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
	_Runtime.o\
	_Hash.o\
	_Loader.o\
	_CriaId.o\
	_CriaObject.o\
	_CriaBlock.o\
	_CriaClass.o\
	_CriaIO.o\
	_CriaString.o\
	_CriaInteger.o\
	_CriaBoolean.o\
	_CriaFile.o\
	_CriaList.o\

all:$(OBJS)
	$(CC) -shared -Wl,-soname,$(TARGET) -o ../../$(TARGET) $(OBJS) $(LDFLAGS)


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o ../../$(TARGET) $(GCLIB)

_Logger.o: _Logger.c Logger.h
_Memory.o: _Memory.c Memory.h
_String.o: _String.c String.h Memory.h Logger.h
_List.o: _List.c List.h Memory.h Logger.h
_StringBuffer.o: _StringBuffer.c StringBuffer.h Memory.h Logger.h
_Interpreter.o: _Interpreter.c Interpreter.h Memory.h Logger.h
_CriaId.o: _CriaId.c CriaId.h Memory.h Logger.h
_CriaObject.o: _CriaObject.c CriaObject.h Memory.h Logger.h
_CriaBlock.o: _CriaBlock.c CriaBlock.h Memory.h Logger.h
_CriaClass.o: _CriaClass.c CriaClass.h Memory.h Logger.h
_Definition.o: _Definition.c Definition.h Memory.h Logger.h
_Tokenizer.o: _Tokenizer.c Tokenizer.h Memory.h Logger.h
_Parser.o: _Parser.c Parser.h Memory.h Logger.h
_Statement.o: _Statement.c Statement.h Memory.h Logger.h
_Expression.o: _Expression.c Expression.h Memory.h Logger.h
_Reference.o: _Reference.c Reference.h Memory.h Logger.h
_Runtime.o: _Runtime.c Runtime.h Memory.h Logger.h
_Hash.o: _Hash.c Hash.h
_Loader.o: _Loader.c Loader.h Memory.h Logger.h
_CriaString.o: _CriaString.c CriaString.h Memory.h Logger.h
_CriaInteger.o: _CriaInteger.c CriaInteger.h Memory.h Logger.h
_CriaBoolean.o: _CriaBoolean.c CriaBoolean.h Memory.h Logger.h
_CriaFile.o: _CriaFile.c CriaFile.h Memory.h Logger.h
_CriaList.o: _CriaList.c CriaList.h Memory.h Logger.h
_CriaIO.o: _CriaIO.c CriaIO.h Memory.h Logger.h
