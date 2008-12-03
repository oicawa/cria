#ifndef PRIVATE_CRIA_H_INCLUDED
#define PRIVATE_CRIA_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <memory.h>



#include "Cria.h"
#include "_Boolean.h"
#include "_String.h"
#include "_List.h"
#include "_StringBuffer.h"
#include "_Interpreter.h"
#include "_DefinitionVariable.h"
#include "_DefinitionFunction.h"
#include "_DefinitionClass.h"
#include "_CriaFile.h"
#include "_Statement.h"

struct TokenTag
{
    TokenType   type;
    int         row;
    int         column;
    String      buffer;
};



struct TokenizerTag
{
    FILE            *file;
    int             row;
    int             column;
    char            next;
    int             indentLevel;
};



struct ParserTag
{
    List    tokens;
    Item    current;
    Item    next;
    Item    mark;
};



struct StatementTag
{
    StatementKind   kind;
    int             line;
    union {
        StatementFunctionCall   _functionCall_;
        StatementSubstitute     _substitute_;
        StatementIf             _if_;
        StatementWhile          _while_;
        StatementGoto           _goto_;
    } of;
};



typedef enum
{
	GOTO_TYPE_CONTINUE,
	GOTO_TYPE_BREAK,
	GOTO_TYPE_LABEL,
	GOTO_TYPE_RETURN,
} GotoType;



struct StatementGotoTag
{
	GotoType type;
	union
	{
	    String  label;
	    Expression expression;
	} of;
};



struct StatementIfTag
{
    Expression  condition;
    List        statements;
    StatementIf _if_;
};



struct StatementWhileTag
{
    Expression  condition;
    List        statements;
};



struct StatementSubstituteTag
{
    Reference   reference;
    Expression  expression;
};



struct StatementFunctionCallTag
{
    ExpressionReference  expression;
};



struct ExpressionTag
{
    ExpressionKind  kind;
    union {
        ExpressionFunctionCall      _functionCall_;
        ExpressionStringLiteral     _stringLiteral_;
        ExpressionIntegerLiteral    _integerLiteral_;
        ExpressionBooleanLiteral    _booleanLiteral_;
        ExpressionOperation         _operation_;
        ExpressionGenerate          _generate_;
        ExpressionVariable          _variable_;
        ExpressionReference         _reference_;
    } of;
};



struct ExpressionOperationTag
{
    OperationKind   kind;
    Expression      left;
    Expression      right;
};



struct ExpressionFunctionCallTag
{
    String                  name;
    ExpressionParameters    parameters;
};



struct ExpressionGenerateTag
{
    String                  name;
    ExpressionParameters    parameters;
};



struct ExpressionParametersTag
{
    List    list;
};



struct ExpressionStringLiteralTag
{
    String  value;
};



struct ExpressionIntegerLiteralTag
{
    int     value;
};



struct ExpressionBooleanLiteralTag
{
    Boolean value;
};



struct ExpressionVariableTag
{
    String  name;
};



typedef enum
{
    REFERENCE_EXPRESSION_TYPE_SELF,
    REFERENCE_EXPRESSION_TYPE_VARIABLE,
    REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL,
    REFERENCE_EXPRESSION_TYPE_CLASS,
    REFERENCE_EXPRESSION_TYPE_GENERATE,
} ExpressionReferenceType;



struct ExpressionReferenceTag
{
    ExpressionReferenceType     type;
    union {
        ExpressionVariable      variable;
        ExpressionFunctionCall  function;
        ExpressionGenerate      generate;
    } of;
    ExpressionReference         next;
};



struct ReferenceVariableTag
{
    String  name;
};



struct ReferenceFunctionCallTag
{
    String                  name;
    ExpressionParameters    parameters;
};



struct ReferenceClassTag
{
    String  name;
};



typedef enum
{
    REFERENCE_TYPE_SELF,
    REFERENCE_TYPE_VARIABLE,
    REFERENCE_TYPE_FUNCTION_CALL,
    REFERENCE_TYPE_CLASS,
} ReferenceType;



struct ReferenceTag
{
    ReferenceType               type;
    union {
        ReferenceVariable       variable;
        ReferenceFunctionCall   function;
        ReferenceClass          klass;
    } of;
    Reference                   next;
};



List
tokenizer_create_tokens(
    char*   filePath
);



Boolean
parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter
);



Statement
statement_new(
    StatementKind   kind
);



void
statement_dispose(
    Statement   statement
);



Expression
expression_new(
    ExpressionKind  kind
);



void
expression_dispose(
    Expression  expression
);



Reference
reference_new(
    ReferenceType   type
);



//==================================================
//OperationExpression
//==================================================
ExpressionOperation
operationExpression_new(
    OperationKind   kind,
    Expression      left,
    Expression      right
);



void
operationExpression_dispose(
    ExpressionOperation expression
);



//==================================================
//StringLiteralExpression
//==================================================
void
stringLiteralExpression_dispose(
    ExpressionStringLiteral expression
);



ExpressionStringLiteral
stringLiteralExpression_parse(
    Parser  parser
);



//==================================================
//FunctionCallExpression
//==================================================
void
functionCallExpression_dispose(
    ExpressionFunctionCall  expression
);



ExpressionFunctionCall
functionCallExpression_parse(
    Parser  parser
);



//==================================================
//ParametersExpression
//==================================================
ExpressionParameters
parametersExpression_parse(
    Parser  parser
);



void
parametersExpression_dispose(
    ExpressionParameters    expression
);



//==================================================
//VariableExpression
//==================================================
ExpressionVariable
variableExpression_new(
    String                  name
);



void
variableExpression_dispose(
    ExpressionVariable   expression
);



ExpressionVariable
variableExpression_parse(
    Parser  parser
);



//==================================================
//ClassDefinition
//==================================================
Boolean
classDefinition_isMatch(
    Parser parser
);



//==================================================
//IO
//==================================================
CriaId
io_write(
    Interpreter interpreter,
    CriaId object,
    List        args
);



CriaId
io_read(
    Interpreter interpreter,
    CriaId object,
    List        args
);



//==================================================
//Executor
//==================================================
StatementResult
executor_executeStatement(
    Interpreter interpreter,
    CriaId object, 
    List parameters,
    Statement   statement
);



void
executor_executeFunctionCallStatement(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    StatementFunctionCall statement
);



void
executor_executeSubstituteStatement(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    StatementSubstitute statement
);



//==================================================
//Evaluator
//==================================================
CriaId
evaluator_expression(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    Expression              expression
);



DefinitionVariable
evaluator_reference(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    Reference   reference
);



CriaId
evaluator_referenceExpression(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ExpressionReference expression
);



CriaId
evaluator_functionCall(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionFunctionCall  expression
);



#define token_log(token) \
    do \
    { \
    	Logger_dbg("token is [%p]", token); \
        char* buffer = NULL; \
        if (token == NULL) \
            Logger_dbg("Token type=NULL."); \
        else if (token->buffer != NULL) \
        { \
            buffer = token->buffer->pointer; \
            Logger_dbg("Token type=%2d(%3d, %2d) [%s]", token->type, token->row, token->column, buffer); \
        } \
    } while(0) \




#define tokenizer_error(buffer, row, column) \
    do { \
        fprintf(stderr, "Syntax error near '%s'. [%s, %d]\n", buffer, __FILE__, __LINE__); \
        Logger_err("Token error near '%s'. (line:%d, column:%d) [%s, %d]\n", buffer, row, column, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#define parser_error(token) \
    do { \
        Logger_err("Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", (token)->buffer->pointer, (token)->row, (token)->column, __FILE__, __LINE__); \
        fprintf(stderr, "Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", (token)->buffer->pointer, (token)->row, (token)->column, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#endif /* PRIVATE_CRIA_H_INCLUDED */
