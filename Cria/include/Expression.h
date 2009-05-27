#ifndef PUBLIC_EXPRESSION_H_INCLUDED
#define PUBLIC_EXPRESSION_H_INCLUDED



#include "Parser.h"
#include "CriaId.h"
#include "Interpreter.h"
#include "Definition.h"
#include "String.h"



typedef enum
{
    EXPRESSION_KIND_STRING_LITERAL,
    EXPRESSION_KIND_INTEGER_LITERAL,
    EXPRESSION_KIND_REAL_LITERAL,
    EXPRESSION_KIND_BOOLEAN_LITERAL,
    EXPRESSION_KIND_NULL,
    EXPRESSION_KIND_GENERATE,
    EXPRESSION_KIND_FUNCTION_CALL,
    EXPRESSION_KIND_VARIABLE,
    EXPRESSION_KIND_OPERATION,
    EXPRESSION_KIND_REFERENCE,
    EXPRESSION_KIND_BLOCK,
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
typedef struct  ExpressionClassTag              *ExpressionClass;
typedef struct  ExpressionOperationTag          *ExpressionOperation;
typedef struct  ExpressionVariableTag           *ExpressionVariable;
typedef struct  ExpressionReferenceTag          *ExpressionReference;
typedef struct  ExpressionFunctionCallTag       *ExpressionFunctionCall;
typedef struct  ExpressionIndexerTag            *ExpressionIndexer;
typedef struct  ExpressionGenerateTag           *ExpressionGenerate;
typedef struct  ExpressionStringLiteralTag      *ExpressionStringLiteral;
typedef struct  ExpressionIntegerLiteralTag     *ExpressionIntegerLiteral;
typedef struct  ExpressionBooleanLiteralTag     *ExpressionBooleanLiteral;
typedef struct  ExpressionParametersTag         *ExpressionParameters;
typedef struct  ExpressionBlockTag              *ExpressionBlock;



typedef enum
{
    REFERENCE_EXPRESSION_TYPE_SELF,
    REFERENCE_EXPRESSION_TYPE_VARIABLE,
    REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL,
    REFERENCE_EXPRESSION_TYPE_INDEXER,
    REFERENCE_EXPRESSION_TYPE_CLASS,
    REFERENCE_EXPRESSION_TYPE_GENERATE,
} ExpressionReferenceType;



DefinitionFunction
ExpressionFunctionCall_searchFromObject(
	Interpreter interpreter,
    CriaId object,
    String functionName
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
    Expression  expression
);



List
ExpressionParameters_get_list(
	ExpressionParameters parameters
);



#endif
