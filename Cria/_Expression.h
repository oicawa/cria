#ifndef PRIVATE_EXPRESSION_H_INCLUDED
#define PRIVATE_EXPRESSION_H_INCLUDED


#include "Expression.h"


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
    Boolean isStatic;
    Boolean isConstant;
};



struct ExpressionClassTag
{
    String  name;
};



struct ExpressionReferenceTag
{
    ExpressionReferenceType     type;
    union {
        ExpressionClass      klass;
        ExpressionVariable      variable;
        ExpressionFunctionCall  function;
        ExpressionGenerate      generate;
    } of;
    ExpressionReference         next;
};



CriaId
ExpressionFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionFunctionCall expression,
    CriaId parent
);



CriaId
ExpressionGenerate_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameterList,
    ExpressionGenerate  expression
);



CriaId
ExpressionBooleanLiteral_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionBooleanLiteral expression
);



CriaId
ExpressionIntegerLiteral_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionIntegerLiteral expression
);



CriaId
ExpressionStringLiteral_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionStringLiteral expression
);



#endif
