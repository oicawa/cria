#ifndef PRIVATE_EVALUATOR_H_INCLUDED
#define PRIVATE_EVALUATOR_H_INCLUDED

#include "_Reference.h"

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



#endif
