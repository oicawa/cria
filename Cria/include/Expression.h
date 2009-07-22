#ifndef PUBLIC_EXPRESSION_H_INCLUDED
#define PUBLIC_EXPRESSION_H_INCLUDED



#include "Cria.h"
#include "Parser.h"
#include "CriaId.h"
#include "String.h"
#include "Interpreter.h"
#include "Definition.h"



void*
ExpressionReference_getReference(
    ExpressionReference reference
);



ExpressionReference
ExpressionReference_parse(
	Parser parser
);



Expression
Expression_parse(
	Parser parser
);



ExpressionFunctionCall
ExpressionFunctionCall_new(
    String name,
    ExpressionParameters parameters
);



CriaId
ExpressionFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionBlock block,
    ExpressionFunctionCall expression,
    CriaId parent
);



ExpressionReference
ExpressionFunctionCall_parse(
    Parser parser
);



ExpressionReference
ExpressionIndexer_parse(
    Parser parser
);



CriaId
ExpressionReference_evaluate(
    Interpreter         interpreter,
    CriaId variable,
    List parameters,
    ExpressionBlock block,
    ExpressionReference expression,
    CriaId parent
);



ExpressionParameters
ExpressionParameters_parse(
    Parser  parser
);



CriaId
Expression_evaluate(
    Interpreter interpreter,
    CriaId object,
	List parameters,
    ExpressionBlock block,
    Expression  expression
);



List
ExpressionParameters_get_list(
	ExpressionParameters parameters
);



#endif
