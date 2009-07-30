#ifndef PUBLIC_EXPRESSION_H_INCLUDED
#define PUBLIC_EXPRESSION_H_INCLUDED



#include "Cria.h"
#include "Parser.h"
#include "CriaId.h"
#include "String.h"
#include "Interpreter.h"
#include "Definition.h"



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
        ExpressionBlock             _block_;
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
    String name;
    ExpressionParameters parameters;
    ExpressionBlock block;
};



struct ExpressionIndexerTag
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
        ExpressionClass         klass;
        ExpressionVariable      variable;
        ExpressionFunctionCall  function;
        ExpressionIndexer       indexer;
        ExpressionGenerate      generate;
    } of;
    ExpressionReference         next;
};



struct ExpressionBlockTag
{
//    ExpressionParameters parameters;
    DefinitionFunction function;
};



CriaId
ExpressionFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionBlock block,
    ExpressionFunctionCall expression,
    CriaId parent
);



CriaId
ExpressionIndexer_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionIndexer expression,
    CriaId parent
);



CriaId
ExpressionGenerate_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameterList,
    ExpressionBlock block,
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
