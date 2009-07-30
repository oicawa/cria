#ifndef PUBLIC_STATEMENT_H_INCLUDED
#define PUBLIC_STATEMENT_H_INCLUDED

#include "Cria.h"
#include "Parser.h"
#include "Expression.h"
#include "Reference.h"
#include "String.h"
#include "CriaId.h"




struct StatementTag
{
    StatementKind kind;
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



typedef struct {
    StatementResultType type;
    union {
        String label;
        CriaId id;
    } returns;
} StatementResult;



Statement
Statement_parse(
	Parser parser
);



StatementResult
Statement_executeList(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    List statements
);



#endif
