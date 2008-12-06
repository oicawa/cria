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
};



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



#endif
