#ifndef PUBLIC_CRIA_H_INCLUDED
#define PUBLIC_CRIA_H_INCLUDED

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_Boolean.h"
#include "_String.h"
#include "_Interpreter.h"
//#include "_DefinitionClass.h"


typedef enum
{
    INTERPRETER_COMPLE_RESULT_SUCCESS,
    INTERPRETER_COMPLE_RESULT_FAILURE,
} InterpreterCompleResult;



#define INTERPRETER_RUN_RESULT_SUCCESS      0

#define STATEMENT_PARSE_RESULT_SUCCESS      0
#define STATEMENT_PARSE_RESULT_ERROR        -1


//インスタンスへのポインタ
typedef struct  TokenizerTag                    *Tokenizer;
typedef struct  TokenTag                        *Token;
typedef struct  ParserTag                       *Parser;

typedef struct  StatementTag                    *Statement;
typedef struct  StatementFunctionCallTag        *StatementFunctionCall;
typedef struct  StatementSubstituteTag          *StatementSubstitute;
typedef struct  StatementIfTag                  *StatementIf;
typedef struct  StatementWhileTag               *StatementWhile;
typedef struct  StatementGotoTag               *StatementGoto;

typedef struct  ExpressionTag                   *Expression;
typedef struct  ExpressionOperationTag          *ExpressionOperation;
typedef struct  ExpressionVariableTag           *ExpressionVariable;
typedef struct  ExpressionReferenceTag          *ExpressionReference;
typedef struct  ExpressionFunctionCallTag       *ExpressionFunctionCall;
typedef struct  ExpressionGenerateTag           *ExpressionGenerate;
typedef struct  ExpressionStringLiteralTag      *ExpressionStringLiteral;
typedef struct  ExpressionIntegerLiteralTag     *ExpressionIntegerLiteral;
typedef struct  ExpressionBooleanLiteralTag     *ExpressionBooleanLiteral;
typedef struct  ExpressionParametersTag         *ExpressionParameters;

typedef struct  ReferenceTag                    *Reference;
typedef struct  ReferenceVariableTag            *ReferenceVariable;
typedef struct  ReferenceFunctionCallTag        *ReferenceFunctionCall;
typedef struct  ReferenceClassTag               *ReferenceClass;



typedef enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_CONSTANT,
    TOKEN_TYPE_CLASS_LITERAL,
    TOKEN_TYPE_INDENT,
    TOKEN_TYPE_DEDENT,
    TOKEN_TYPE_INTEGER_LITERAL,
    TOKEN_TYPE_BOOLEAN_LITERAL,
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
    TOKEN_TYPE_GOTO,
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
    TOKEN_TYPE_DUMMY,
} TokenType;



//ステートメントタイプ
typedef enum
{
    STATEMENT_KIND_SUBSTITUTE,
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
    EXPRESSION_KIND_STRING_LITERAL,
    EXPRESSION_KIND_INTEGER_LITERAL,
    EXPRESSION_KIND_REAL_LITERAL,
    EXPRESSION_KIND_BOOLEAN_LITERAL,
    EXPRESSION_KIND_NULL_LITERAL,
    EXPRESSION_KIND_GENERATE,
    EXPRESSION_KIND_FUNCTION_CALL,
    EXPRESSION_KIND_VARIABLE,
    EXPRESSION_KIND_OPERATION,
    EXPRESSION_KIND_REFERENCE,
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
    OPERATION_KIND_NOT_EQUAL,
    OPERATION_KIND_OR,
    OPERATION_KIND_AND,
} OperationKind;



#endif //PUBLIC_CRIA_H_INCLUDED
