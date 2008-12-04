#ifndef PRIVATE_CRIAINTEGER_H_INCLUDED
#define PRIVATE_CRIAINTEGER_H_INCLUDED


#include "_Boolean.h"
#include "_CriaId.h"
#include "_Interpreter.h"
#include "_Expression.h"

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
