#ifndef PRIVATE_STATEMENT_H_INCLUDED
#define PRIVATE_STATEMENT_H_INCLUDED


#include "String.h"
#include "CriaId.h"

#include "Expression.h"
#include "Reference.h"
#include "Statement.h"

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



struct StatementTag
{
    StatementKind   kind;
    int             line;
    union {
        StatementFunctionCall   _functionCall_;
        StatementSubstitute     _substitute_;
        StatementReference      _reference_;
        StatementIf             _if_;
        StatementWhile          _while_;
        StatementGoto           _goto_;
    } of;
};



struct StatementFunctionCallTag
{
    ExpressionReference  expression;
};



struct StatementSubstituteTag
{
    Reference   reference;
    Expression  expression;
};



struct StatementReferenceTag
{
    Reference   reference;
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



Statement
Statement_new(
    StatementKind   kind
);



#endif
