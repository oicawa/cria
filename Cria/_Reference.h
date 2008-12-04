#ifndef PRIVATE_REFERENCE_H_INCLUDED
#define PRIVATE_REFERENCE_H_INCLUDED



#include "_String.h"
#include "_Parser.h"
#include "_Expression.h"



typedef struct  ReferenceTag                    *Reference;
typedef struct  ReferenceVariableTag            *ReferenceVariable;
typedef struct  ReferenceFunctionCallTag        *ReferenceFunctionCall;
typedef struct  ReferenceClassTag               *ReferenceClass;



struct ReferenceVariableTag
{
    String  name;
};



struct ReferenceFunctionCallTag
{
    String                  name;
    ExpressionParameters    parameters;
};



struct ReferenceClassTag
{
    String  name;
};



typedef enum
{
    REFERENCE_TYPE_SELF,
    REFERENCE_TYPE_VARIABLE,
    REFERENCE_TYPE_FUNCTION_CALL,
    REFERENCE_TYPE_CLASS,
} ReferenceType;



struct ReferenceTag
{
    ReferenceType               type;
    union {
        ReferenceVariable       variable;
        ReferenceFunctionCall   function;
        ReferenceClass          klass;
    } of;
    Reference                   next;
};



Reference
parser_reference(
	Parser parser
);



Reference
reference_new(
    ReferenceType   type
);



#endif
