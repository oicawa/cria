#ifndef PRIVATE_REFERENCE_H_INCLUDED
#define PRIVATE_REFERENCE_H_INCLUDED



#include "String.h"
#include "Expression.h"


#include "Reference.h"



struct ReferenceVariableTag
{
    String  name;
    Expression value;
};



struct ReferenceIndexerTag
{
    ExpressionParameters parameters;
    Expression value;
};



struct ReferenceClassTag
{
    String  name;
};



struct ReferenceTag
{
    ReferenceType type;
    union {
        ReferenceVariable variable;
        ExpressionFunctionCall function;
        ReferenceIndexer indexer;
        ReferenceClass klass;
    } of;
    Reference next;
};



#endif
