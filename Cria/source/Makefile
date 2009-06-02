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
	_CriaVariable.o\

all:$(OBJS)
	$(CC) -shared -Wl,-soname,$(TARGET) -o ../../$(TARGET) $(OBJS) $(LDFLAGS)


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o ../../$(TARGET) $(GCLIB)

_Logger.o: _Logger.c _Logger.h ../include/Logger.h
_Memory.o: _Memory.c _Memory.h ../include/Memory.h
_String.o: _String.c _String.h ../include/Memory.h ../include/Logger.h
_List.o: _List.c _List.h ../include/Memory.h ../include/Logger.h
_StringBuffer.o: _StringBuffer.c _StringBuffer.h ../include/Memory.h ../include/Logger.h
_Interpreter.o: _Interpreter.c _Interpreter.h ../include/Memory.h ../include/Logger.h
_CriaId.o: _CriaId.c _CriaId.h ../include/Memory.h ../include/Logger.h
_CriaObject.o: _CriaObject.c _CriaObject.h ../include/Memory.h ../include/Logger.h
_CriaBlock.o: _CriaBlock.c _CriaBlock.h ../include/Memory.h ../include/Logger.h
_CriaClass.o: _CriaClass.c _CriaClass.h ../include/Memory.h ../include/Logger.h
_Definition.o: _Definition.c _Definition.h ../include/Memory.h ../include/Logger.h
_Tokenizer.o: _Tokenizer.c _Tokenizer.h ../include/Memory.h ../include/Logger.h
_Parser.o: _Parser.c _Parser.h ../include/Memory.h ../include/Logger.h
_Statement.o: _Statement.c _Statement.h ../include/Memory.h ../include/Logger.h
_Expression.o: _Expression.c _Expression.h ../include/Memory.h ../include/Logger.h
_Reference.o: _Reference.c _Reference.h ../include/Memory.h ../include/Logger.h
_Hash.o: _Hash.c _Hash.h ../include/Hash.h
_Loader.o: _Loader.c _Loader.h ../include/Memory.h ../include/Logger.h
_CriaString.o: _CriaString.c _CriaString.h ../include/Memory.h ../include/Logger.h
_CriaInteger.o: _CriaInteger.c _CriaInteger.h ../include/Memory.h ../include/Logger.h
_CriaBoolean.o: _CriaBoolean.c _CriaBoolean.h ../include/Memory.h ../include/Logger.h
_CriaFile.o: _CriaFile.c _CriaFile.h ../include/Memory.h ../include/Logger.h
_CriaList.o: _CriaList.c _CriaList.h ../include/Memory.h ../include/Logger.h
_CriaIO.o: _CriaIO.c _CriaIO.h ../include/Memory.h ../include/Logger.h
_CriaVariable.o: _CriaVariable.c _CriaVariable.h ../include/Memory.h ../include/Logger.h
