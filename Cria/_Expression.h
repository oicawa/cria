#ifndef PRIVATE_EXPRESSION_H_INCLUDED
#define PRIVATE_EXPRESSION_H_INCLUDED



#include "_Parser.h"


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






ExpressionStringLiteral
stringLiteralExpression_parse(
    Parser  parser
);


ExpressionParameters
parametersExpression_parse(
    Parser  parser
);



ExpressionOperation
operationExpression_new(
    OperationKind   kind,
    Expression      left,
    Expression      right
);



Expression
parser_expression(
	Parser parser
);



ExpressionReference
parser_expression_reference(
	Parser parser
);



Expression
expression_new(
    ExpressionKind  kind
);



void
expression_dispose(
    Expression  expression
);



void
operationExpression_dispose(
    ExpressionOperation expression
);



void
stringLiteralExpression_dispose(
    ExpressionStringLiteral expression
);



void
functionCallExpression_dispose(
    ExpressionFunctionCall  expression
);



ExpressionFunctionCall
functionCallExpression_parse(
    Parser  parser
);



void
parametersExpression_dispose(
    ExpressionParameters    expression
);



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



#endif
