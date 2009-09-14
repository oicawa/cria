/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
#ifndef PUBLIC_CRIA_H_INCLUDED
#define PUBLIC_CRIA_H_INCLUDED

#include <assert.h>
#include <ctype.h>
#include <dlfcn.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <gc/gc.h>
#include <wchar.h>
#include <wctype.h>




//==============================
//Cria
//==============================
typedef enum
{
    CRIA_DATA_TYPE_NULL,
    CRIA_DATA_TYPE_VOID,
    CRIA_DATA_TYPE_BOOLEAN,
    CRIA_DATA_TYPE_INTEGER,
    CRIA_DATA_TYPE_STRING,
	CRIA_DATA_TYPE_FILE,
    CRIA_DATA_TYPE_LIST,
    CRIA_DATA_TYPE_FUNCTION,
    CRIA_DATA_TYPE_CRIA_OBJECT,
    CRIA_DATA_TYPE_CRIA_CLASS,
    CRIA_DATA_TYPE_CRIA_FUNCTION,
    CRIA_DATA_TYPE_VARIABLE,
    CRIA_DATA_TYPE_BLOCK,
} CriaDataType;


typedef struct CriaIdTag *CriaId;

typedef struct CriaBlockTag *CriaBlock;
typedef struct CriaClassTag *CriaClass;
typedef struct CriaObjectTag *CriaObject;
typedef struct CriaStringTag  *CriaString;


//==============================
//String
//==============================
typedef wchar_t* String;


//==============================
//List
//==============================
typedef struct ItemTag *Item;
typedef struct ListTag *List;


//==============================
//StringBuffer
//==============================
typedef struct  ListTag *StringBuffer;


//==============================
//Interpreter
//==============================
typedef struct InterpreterTag *Interpreter;

//==============================
//Boolean
//==============================
typedef enum
{
    FALSE,
    TRUE,
} Boolean;


//==============================
//Hash
//==============================
typedef struct EntryTag *Entry;
typedef struct HashTag *Hash;

//==============================
//Expression
//==============================
typedef enum
{
    EXPRESSION_KIND_NULL,
    EXPRESSION_KIND_GENERATE,
    EXPRESSION_KIND_FUNCTION_CALL,
    EXPRESSION_KIND_VARIABLE,
    EXPRESSION_KIND_OPERATION,
    EXPRESSION_KIND_REFERENCE,
    EXPRESSION_KIND_BLOCK,
} ExpressionKind;



typedef enum
{
    OPERATION_KIND_PLUS,
    OPERATION_KIND_MINUS,
    OPERATION_KIND_MULTIPLY,
    OPERATION_KIND_DIVIDE,
    OPERATION_KIND_MODULO,
    OPERATION_KIND_EQUAL,
    OPERATION_KIND_LESS_THAN,
    OPERATION_KIND_LESS_EQUAL,
    OPERATION_KIND_NOT_EQUAL,
    OPERATION_KIND_OR,
    OPERATION_KIND_AND,
} OperationKind;



typedef enum
{
    REFERENCE_EXPRESSION_TYPE_SELF,
    REFERENCE_EXPRESSION_TYPE_VARIABLE,
    REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL,
    REFERENCE_EXPRESSION_TYPE_INDEXER,
    REFERENCE_EXPRESSION_TYPE_CLASS,
    REFERENCE_EXPRESSION_TYPE_GENERATE,
    REFERENCE_EXPRESSION_TYPE_STRING,
    REFERENCE_EXPRESSION_TYPE_INTEGER,
    REFERENCE_EXPRESSION_TYPE_BOOLEAN,
} ExpressionReferenceType;



typedef struct  ExpressionTag                   *Expression;
typedef struct  ExpressionClassTag              *ExpressionClass;
typedef struct  ExpressionOperationTag          *ExpressionOperation;
typedef struct  ExpressionVariableTag           *ExpressionVariable;
typedef struct  ExpressionReferenceTag          *ExpressionReference;
typedef struct  ExpressionFunctionCallTag       *ExpressionFunctionCall;
typedef struct  ExpressionIndexerTag            *ExpressionIndexer;
typedef struct  ExpressionGenerateTag           *ExpressionGenerate;
typedef struct  ExpressionStringLiteralTag      *ExpressionStringLiteral;
typedef struct  ExpressionIntegerLiteralTag     *ExpressionIntegerLiteral;
typedef struct  ExpressionBooleanLiteralTag     *ExpressionBooleanLiteral;
typedef struct  ExpressionParametersTag         *ExpressionParameters;
typedef struct  ExpressionBlockTag              *ExpressionBlock;



//==============================
//Definition
//==============================
typedef enum 
{
    DEFINITION_VARIABLE_TYPE_NORMAL,
    DEFINITION_VARIABLE_TYPE_ITEM,
} DefinitionVariableType;



typedef struct DefinitionVariableTag *DefinitionVariable;
typedef struct DefinitionVariableNormalTag *DefinitionVariableNormal;
typedef struct DefinitionClassTag *DefinitionClass;
typedef struct DefinitionFunctionTag *DefinitionFunction;
typedef DefinitionClass CriaNativeClassLoader(String name);


//==============================
//Loader
//==============================
typedef struct LoaderTag *Loader;


//==============================
//Logger
//==============================
#define LOG_LEVEL_ERROR         0
#define LOG_LEVEL_WARNING       1
#define LOG_LEVEL_INFORMATION   2
#define LOG_LEVEL_TRACE         3
#define LOG_LEVEL_DEBUG         4
#define LOG_LEVEL_CORE          5



//==============================
//Parser
//==============================
typedef struct  ParserTag *Parser;


//==============================
//Reference
//==============================
typedef enum
{
	REFERENCE_TYPE_SELF,
    REFERENCE_TYPE_VARIABLE,
    REFERENCE_TYPE_FUNCTION_CALL,
    REFERENCE_TYPE_INDEXER,
    REFERENCE_TYPE_CLASS,
    REFERENCE_TYPE_LITERAL,
} ReferenceType;


typedef enum
{
	REFERENCE_LITERAL_TYPE_STRING,
	REFERENCE_LITERAL_TYPE_INTEGER,
	REFERENCE_LITERAL_TYPE_BOOLEAN,
} ReferenceLiteralType;


typedef struct  ReferenceTag                    *Reference;
typedef struct  ReferenceVariableTag            *ReferenceVariable;
typedef struct  ReferenceFunctionCallTag        *ReferenceFunctionCall;
typedef struct  ReferenceIndexerTag             *ReferenceIndexer;
typedef struct  ReferenceClassTag               *ReferenceClass;
typedef struct  ReferenceLiteralTag             *ReferenceLiteral;



//==============================
//Statement
//==============================
typedef enum
{
    STATEMENT_KIND_SUBSTITUTE,
    STATEMENT_KIND_REFERENCE,
    STATEMENT_KIND_FUNCTION_CALL,
    STATEMENT_KIND_IF,
    STATEMENT_KIND_WHILE,
    STATEMENT_KIND_FOR,
    STATEMENT_KIND_GOTO,
    STATEMENT_KIND_CATCH,
    STATEMENT_KIND_FINALLY,
    STATEMENT_KIND_VARIABLE_DEFINITION,
    STATEMENT_KIND_FUNCTION_DEFINITION,
    STATEMENT_KIND_CLASS_DEFINITION,
} StatementKind;



typedef enum
{
	GOTO_TYPE_CONTINUE,
	GOTO_TYPE_BREAK,
	GOTO_TYPE_LABEL,
	GOTO_TYPE_RETURN,
} GotoType;



typedef struct StatementTag *Statement;
typedef struct StatementFunctionCallTag *StatementFunctionCall;
typedef struct StatementSubstituteTag *StatementSubstitute;
typedef struct StatementReferenceTag *StatementReference;
typedef struct StatementGotoTag *StatementGoto;
typedef struct StatementIfTag *StatementIf;
typedef struct StatementWhileTag *StatementWhile;


typedef enum {
    STATEMENT_RESULT_NORMAL = 1,
    STATEMENT_RESULT_RETURN,
    STATEMENT_RESULT_BREAK,
    STATEMENT_RESULT_CONTINUE,
    STATEMENT_RESULT_LABEL,
} StatementResultType;



//==============================
//Tokenizer
//==============================
typedef enum
{
    TOKEN_TYPE_AND,
    TOKEN_TYPE_ATMARK,
    TOKEN_TYPE_BOOLEAN_LITERAL_TRUE,
    TOKEN_TYPE_BOOLEAN_LITERAL_FALSE,
    TOKEN_TYPE_BLOCK,
    TOKEN_TYPE_BRACE_LEFT,
    TOKEN_TYPE_BRACE_RIGHT,
    TOKEN_TYPE_BRACKET_LEFT,
    TOKEN_TYPE_BRACKET_RIGHT,
    TOKEN_TYPE_BREAK,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_CATCH,
    TOKEN_TYPE_CLASS,
    TOKEN_TYPE_CLASS_LITERAL,
    TOKEN_TYPE_CONSTANT,
    TOKEN_TYPE_CONTINUE,
    TOKEN_TYPE_DECREMENT,
    TOKEN_TYPE_DEDENT,
    TOKEN_TYPE_DEVIDE,
    TOKEN_TYPE_DUMMY,
    TOKEN_TYPE_ELIF,
    TOKEN_TYPE_ELSE,
    TOKEN_TYPE_EQUAL,
    TOKEN_TYPE_EXTENDS,
    TOKEN_TYPE_FINALLY,
    TOKEN_TYPE_FOR,
    TOKEN_TYPE_GENERICS_LEFT,
    TOKEN_TYPE_GENERICS_RIGHT,
    TOKEN_TYPE_GOTO,
    TOKEN_TYPE_GREATER_EQUAL,
    TOKEN_TYPE_GREATER_THAN,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_IF,
    TOKEN_TYPE_IN,
    TOKEN_TYPE_INCREMENT,
    TOKEN_TYPE_INDENT,
    TOKEN_TYPE_INTEGER_LITERAL,
    TOKEN_TYPE_JOIN,
    TOKEN_TYPE_LESS_THAN,
    TOKEN_TYPE_LESS_EQUAL,
    TOKEN_TYPE_LOAD,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_MODULO,
    TOKEN_TYPE_MONADIC_MINUS,
    TOKEN_TYPE_MULTIPLY,
    TOKEN_TYPE_NEW_LINE,
    TOKEN_TYPE_NOT_EQUAL,
    TOKEN_TYPE_NULL,
    TOKEN_TYPE_OR,
    TOKEN_TYPE_PACKAGE,
    TOKEN_TYPE_PARENTHESIS_LEFT,
    TOKEN_TYPE_PARENTHESIS_RIGHT,
    TOKEN_TYPE_PERIOD,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_RETURN,
    TOKEN_TYPE_RETURN_VALUE,
    TOKEN_TYPE_STRING_LITERAL,
    TOKEN_TYPE_SUBSTITUTE,
    TOKEN_TYPE_THROW,
    TOKEN_TYPE_TERMINATE,
    TOKEN_TYPE_WHILE,
    TOKEN_TYPE_UNDER,
    TOKEN_COUNT,
} TokenType;


typedef struct  TokenTag *Token;
typedef struct  TokenizerTag *Tokenizer;


typedef CriaId CriaNativeFunction(Interpreter interpreter, CriaId object, List args, CriaBlock block);





struct CriaIdTag
{
    String          name;
    CriaDataType    type;
    int             refCount;
};



struct CriaStringTag
{
    struct CriaIdTag    id;
    Boolean             isLiteral;
    String              value;
};





Interpreter
Interpreter_new(
    void
);



Boolean
Interpreter_compile(
    Interpreter interpreter,
    String filePath
);



void
Interpreter_run(
    Interpreter interpreter
);



void
Interpreter_dispose(
	Interpreter interpreter
);



DefinitionVariable
DefinitionVariable_new(
    DefinitionVariableType type,
    String name,
    Boolean isStatic,
    Boolean isConstant,
    Item item
);



DefinitionFunction
DefinitionFunction_new(
    String              name,
    Boolean             isNative,
    Boolean             isStatic,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
);



DefinitionClass
DefinitionClass_new(
    String name,
    Boolean isNative,
    Hash i_fields,
    Hash s_fields,
    Hash i_methods,
    Hash s_methods,
    CriaNativeClassLoader* classLoader
);



String
DefinitionFunction_get_name(
	DefinitionFunction function
);



CriaObject
CriaObject_new(
	String className
);



void
CriaObject_addField(
	CriaObject object,
    DefinitionVariable variable
);



void*
CriaObject_get(
	Interpreter interpreter,
	CriaObject object,
    String name
);



void
CriaObject_set(
	Interpreter interpreter,
	CriaObject object,
    String name,
    void* value
);



CriaString
CriaString_new(
    Boolean         isLiteral,
    String          value
);



CriaId
CriaBlock_evaluate(
    CriaBlock block,
    List parameters
);



void
Loader_add_class(
    Interpreter interpreter,
    String className,
    CriaNativeClassLoader* classLoader
);



void
Logger_write(
    char*       fileName,
    const char* funcName,
    int         line,
    int         logLevel,
    char*       format,
    ...
);
#define Logger_err(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_ERROR, __VA_ARGS__))
#define Logger_wrn(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_WARNING, __VA_ARGS__))
#define Logger_inf(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_INFORMATION, __VA_ARGS__))
#define Logger_trc(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_TRACE, __VA_ARGS__))
#define Logger_dbg(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_DEBUG, __VA_ARGS__))
#define Logger_cor(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_CORE, __VA_ARGS__))



List
List_new(
    void
);



int
List_count(
    List    list
);



void
Runtime_error_write(
    Interpreter interpreter,
    const char* file_name,
    const char* func_name,
    int line,
    char*       format,
    ...
);
#define Runtime_error(interpreter, ...) \
    Runtime_error_write(interpreter, __FILE__, __func__, __LINE__, __VA_ARGS__);



void*
Memory_malloc(
    size_t  size
);



void*
List_get(
    List    list,
    int     index
);



Hash
Hash_new(
	int size
);



void
Hash_put(
	Hash hash,
	String key,
	void* object
);



CriaId
ExpressionBlock_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionBlock expression
);



#endif
