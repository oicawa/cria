TARGET = libcria.so
CFLAGS = -fPIC -c -g -DDEBUG -Wall -I../../Library/include/ -I../include
LDFLAGS = -L../../Library/lib -lgc
GC = gc
GCLIB = lib$(GC).so
OBJS =\
	Memory.o\
	Logger.o\
	String.o\
	List.o\
	StringBuffer.o\
	Interpreter.o\
	Definition.o\
	Tokenizer.o\
	Parser.o\
	Statement.o\
	Expression.o\
	Reference.o\
	Runtime.o\
	Hash.o\
	Loader.o\
	CriaId.o\
	CriaObject.o\
	CriaBlock.o\
	CriaClass.o\
	CriaIO.o\
	CriaString.o\
	CriaInteger.o\
	CriaBoolean.o\
	CriaFile.o\
	CriaList.o\

all:$(OBJS)
	$(CC) -shared -Wl,-soname,$(TARGET) -o ../../$(TARGET) $(OBJS) $(LDFLAGS)


.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o ../../$(TARGET) $(GCLIB)

Logger.o: Logger.c Logger.h
Memory.o: Memory.c Memory.h
String.o: String.c String.h Memory.h Logger.h
List.o: List.c List.h Memory.h Logger.h
StringBuffer.o: StringBuffer.c StringBuffer.h Memory.h Logger.h
Interpreter.o: Interpreter.c Interpreter.h Memory.h Logger.h
CriaId.o: CriaId.c CriaId.h Memory.h Logger.h
CriaObject.o: CriaObject.c CriaObject.h Memory.h Logger.h
CriaBlock.o: CriaBlock.c CriaBlock.h Memory.h Logger.h
CriaClass.o: CriaClass.c CriaClass.h Memory.h Logger.h
Definition.o: Definition.c Definition.h Memory.h Logger.h
Tokenizer.o: Tokenizer.c Tokenizer.h Memory.h Logger.h
Parser.o: Parser.c Parser.h Memory.h Logger.h
Statement.o: Statement.c Statement.h Memory.h Logger.h
Expression.o: Expression.c Expression.h Memory.h Logger.h
Reference.o: Reference.c Reference.h Memory.h Logger.h
Runtime.o: Runtime.c Runtime.h Memory.h Logger.h
Hash.o: Hash.c Hash.h
Loader.o: Loader.c Loader.h Memory.h Logger.h
CriaString.o: CriaString.c CriaString.h Memory.h Logger.h
CriaInteger.o: CriaInteger.c CriaInteger.h Memory.h Logger.h
CriaBoolean.o: CriaBoolean.c CriaBoolean.h Memory.h Logger.h
CriaFile.o: CriaFile.c CriaFile.h Memory.h Logger.h
CriaList.o: CriaList.c CriaList.h Memory.h Logger.h
CriaIO.o: CriaIO.c CriaIO.h Memory.h Logger.h
