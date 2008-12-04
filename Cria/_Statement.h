#ifndef PRIVATE_STATEMENT_H_INCLUDED
#define PRIVATE_STATEMENT_H_INCLUDED


#include "_String.h"
#include "_CriaId.h"
#include "_Expression.h"
#include "_Reference.h"


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



typedef struct  StatementTag                    *Statement;
typedef struct  StatementFunctionCallTag        *StatementFunctionCall;
typedef struct  StatementSubstituteTag          *StatementSubstitute;
typedef struct  StatementIfTag                  *StatementIf;
typedef struct  StatementWhileTag               *StatementWhile;
typedef struct  StatementGotoTag               *StatementGoto;



typedef enum {
    STATEMENT_RESULT_NORMAL = 1,
    STATEMENT_RESULT_RETURN,
    STATEMENT_RESULT_BREAK,
    STATEMENT_RESULT_CONTINUE,
    STATEMENT_RESULT_LABEL,
} StatementResultType;



typedef struct {
    StatementResultType type;
    union {
        String label;
        CriaId id;
    } returns;
} StatementResult;



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



Statement
parser_statement(
	Parser parser
);



Statement
statement_new(
    StatementKind   kind
);



void
statement_dispose(
    Statement   statement
);





#endif
