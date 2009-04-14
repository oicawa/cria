#ifndef PRIVATE_REFERENCE_H_INCLUDED
#define PRIVATE_REFERENCE_H_INCLUDED



#include "String.h"
#include "Expression.h"


#include "Reference.h"



struct ReferenceVariableTag
{
    String  name;
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
        ExpressionFunctionCall  function;
        ReferenceClass          klass;
    } of;
    Reference                   next;
};



#endif
