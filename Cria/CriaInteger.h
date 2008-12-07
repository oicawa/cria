#ifndef PUBLIC_CRIAINTEGER_H_INCLUDED
#define PUBLIC_CRIAINTEGER_H_INCLUDED


#include "Boolean.h"
#include "CriaId.h"
#include "Interpreter.h"
#include "Expression.h"

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



#endif
