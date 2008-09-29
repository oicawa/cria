#ifndef PUBLIC_CRIA_H_INCLUDED
#define PUBLIC_CRIA_H_INCLUDED

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"


typedef enum
{
    FALSE,
    TRUE,
} Boolean;



typedef enum
{
    INTERPRETER_COMPLE_RESULT_SUCCESS,
    INTERPRETER_COMPLE_RESULT_FAILURE,
} InterpreterCompleResult;



#define INTERPRETER_RUN_RESULT_SUCCESS      0

#define STATEMENT_PARSE_RESULT_SUCCESS      0
#define STATEMENT_PARSE_RESULT_ERROR        -1


typedef struct  StringTag       *String;
typedef struct  ItemTag         *Item;
typedef struct  ListTag         *List;
typedef struct  ListTag         *StringBuffer;

//インスタンスへのポインタ
typedef struct  InterpreterTag              *Interpreter;
typedef struct  TokenizerTag                *Tokenizer;
typedef struct  TokenTag                    *Token;
typedef struct  ParserTag                   *Parser;

typedef struct  StatementTag                *Statement;
typedef struct  ReturnStatementTag          *ReturnStatement;
typedef struct  CatchStatementTag           *CatchStatement;
typedef struct  FinallyStatementTag         *FinallyStatement;
typedef struct  IfStatementTag              *IfStatement;
typedef struct  WhileStatementTag           *WhileStatement;
typedef struct  ForStatementTag             *ForStatement;
typedef struct  FunctionCallStatementTag    *FunctionCallStatement;
typedef struct  SubstituteStatementTag      *SubstituteStatement;
typedef struct  VariableDefineTag           *VariableDefinition;
typedef struct  FunctionDefinitionTag       *FunctionDefinition;
typedef struct  ClassDefineTag              *ClassDefinition;

typedef struct  ExpressionTag               *Expression;
typedef struct  OperationExpressionTag      *OperationExpression;
typedef struct  ObjectExpressionTag         *ObjectExpression;
typedef struct  VariableExpressionTag       *VariableExpression;
typedef struct  FunctionCallExpressionTag   *FunctionCallExpression;
typedef struct  StringLiteralExpressionTag  *StringLiteralExpression;
typedef struct  ParametersExpressionTag     *ParametersExpression;



typedef enum
{
    CRIA_DATA_TYPE_NULL,
    CRIA_DATA_TYPE_VOID,
    CRIA_DATA_TYPE_BOOLEAN,
    CRIA_DATA_TYPE_INTEGER,
    CRIA_DATA_TYPE_STRING,
    CRIA_DATA_TYPE_LIST,
    CRIA_DATA_TYPE_FUNCTION,
    CRIA_DATA_TYPE_CRIA_OBJECT,
    CRIA_DATA_TYPE_CRIA_FUNCTION,
} CriaDataType;



typedef struct
{
    String          name;
    CriaDataType    type;
    int             refCount;
} CriaIdentifier;



typedef struct
{
    CriaIdentifier  id;
    Boolean         value;
} CriaBoolean;



typedef struct
{
    CriaIdentifier  id;
    Boolean         isLiteral;
    String          value;
} CriaString;



typedef struct
{
    CriaIdentifier  id;
    void*           object;
} CriaObject;



typedef CriaObject CriaNativeFunction(Interpreter interpreter, List args);



typedef enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_CONSTANT,
    TOKEN_TYPE_CLASS_LITERAL,
    TOKEN_TYPE_INDENT,
    TOKEN_TYPE_DEDENT,
    TOKEN_TYPE_INTEGER_LITERAL,
    TOKEN_TYPE_STRING_LITERAL,
    TOKEN_TYPE_IF,
    TOKEN_TYPE_FOR,
    TOKEN_TYPE_IN,
    TOKEN_TYPE_ELIF,
    TOKEN_TYPE_ELSE,
    TOKEN_TYPE_WHILE,
    TOKEN_TYPE_THROW,
    TOKEN_TYPE_RETURN,
    TOKEN_TYPE_RETURN_VALUE,
    TOKEN_TYPE_BREAK,
    TOKEN_TYPE_CONTINUE,
    TOKEN_TYPE_CATCH,
    TOKEN_TYPE_FINALLY,
    TOKEN_TYPE_SUBSTITUTE,
    TOKEN_TYPE_INCREMENT,
    TOKEN_TYPE_DECREMENT,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_MULTIPLY,
    TOKEN_TYPE_DEVIDE,
    TOKEN_TYPE_MODULO,
    TOKEN_TYPE_PARENTHESIS_LEFT,
    TOKEN_TYPE_PARENTHESIS_RIGHT,
    TOKEN_TYPE_BRACKET_LEFT,
    TOKEN_TYPE_BRACKET_RIGHT,
    TOKEN_TYPE_BRACE_LEFT,
    TOKEN_TYPE_BRACE_RIGHT,
    TOKEN_TYPE_GENERICS_LEFT,
    TOKEN_TYPE_GENERICS_RIGHT,
    TOKEN_TYPE_LESS_THAN,
    TOKEN_TYPE_LESS_EQUAL,
    TOKEN_TYPE_EQUAL,
    TOKEN_TYPE_NOT_EQUAL,
    TOKEN_TYPE_OR,
    TOKEN_TYPE_AND,
    TOKEN_TYPE_GREATER_EQUAL,
    TOKEN_TYPE_GREATER_THAN,
    TOKEN_TYPE_EXTENDS,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_MEMBER,
    TOKEN_TYPE_PERIOD,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_UNDER,
    TOKEN_TYPE_NULL,
    TOKEN_TYPE_MONADIC_MINUS,
    TOKEN_TYPE_NEW_LINE,
} TokenType;



//ステートメントタイプ
typedef enum
{
    STATEMENT_KIND_RETURN,
    STATEMENT_KIND_CATCH,
    STATEMENT_KIND_FINALLY,
    STATEMENT_KIND_IF,
    STATEMENT_KIND_WHILE,
    STATEMENT_KIND_FOR,
    STATEMENT_KIND_VARIABLE_DEFINITION,
    STATEMENT_KIND_FUNCTION_DEFINITION,
    STATEMENT_KIND_CLASS_DEFINITION,
    STATEMENT_KIND_SUBSTITUTE,
    STATEMENT_KIND_FUNCTION_CALL,
} StatementKind;



typedef enum
{
    EXPRESSION_KIND_STRING_LITERAL,
    EXPRESSION_KIND_INTEGER_LITERAL,
    EXPRESSION_KIND_REAL_LITERAL,
    EXPRESSION_KIND_BOOLEAN_LITERAL,
    EXPRESSION_KIND_NULL_LITERAL,
    EXPRESSION_KIND_GENERATE,
    EXPRESSION_KIND_FUNCTION_CALL,
    EXPRESSION_KIND_VARIABLE,
    EXPRESSION_KIND_OPERATION,
} ExpressionKind;



typedef enum
{
    ACCESS_LEVEL_PUBLIC,
    ACCESS_LEVEL_PACKAGE,
    ACCESS_LEVEL_PROTECTED,
    ACCESS_LEVEL_PRIVATE,
} AccessLevel;



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
} OperationKind;



//==============================
//Interpreter
//==============================
//Interpreterを生成
Interpreter
Interpreter_new(
    void
);



//Interpreterを破棄
void
Interpreter_dispose(
    Interpreter interpreter
);



//コンパイル
Boolean
Interpreter_compile(
    Interpreter interpreter,
    char*       filePath
);



//実行
void
Interpreter_run(
    Interpreter interpreter
);



FunctionDefinition
Interpreter_searchFunction(
    Interpreter interpreter,
    char*       name
);



CriaObject*
CriaObject_new(
    String          name,
    CriaDataType    type,
    void*           object
);



void
CriaObject_dispose(
    CriaObject  criaObject
);



CriaString*
CriaString_new(
    String          name,
    Boolean         isLiteral,
    String          value
);



#endif //PUBLIC_CRIA_H_INCLUDED
