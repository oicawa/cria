#ifndef PRIVATE_REFERENCE_H_INCLUDED
#define PRIVATE_REFERENCE_H_INCLUDED



#include "String.h"
#include "Expression.h"


#include "Reference.h"



struct ReferenceVariableTag
{
    String  name;
    Boolean isMember;
};



struct ReferenceFunctionCallTag
{
    String                  name;
    ExpressionParameters    parameters;
    Boolean isMember;
};



struct ReferenceClassTag
{
    String  name;
};



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



#endif
