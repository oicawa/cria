#ifndef PUBLIC_CRIAINTEGER_H_INCLUDED
#define PUBLIC_CRIAINTEGER_H_INCLUDED


#include "CriaId.h"
#include "Boolean.h"
#include "Interpreter.h"
#include "Expression.h"
#include "Definition.h"



typedef struct CriaIntegerTag
{
    struct CriaIdTag    id;
    Boolean             isLiteral;
    int                 value;
} *CriaInteger;



CriaInteger
CriaInteger_new(
    Boolean isLiteral,
    int     value
);



CriaId
CriaInteger_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaInteger     left,
    CriaInteger     right
);



DefinitionClass
CriaInteger_loadClass(
    String className
);



#endif
