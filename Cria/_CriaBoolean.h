#ifndef PRIVATE_CRIABOOLEAN_H_INCLUDED
#define PRIVATE_CRIABOOLEAN_H_INCLUDED


#include "_Boolean.h"
#include "_CriaId.h"



typedef struct CriaBooleanTag
{
    struct CriaIdTag    id;
    Boolean             isLiteral;
    Boolean             value;
} *CriaBoolean;



CriaBoolean
CriaBoolean_new(
    Boolean         isLiteral,
    Boolean         value
);



CriaId
CriaBoolean_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaBoolean     left,
    CriaBoolean     right
);



CriaId
CriaBoolean_toString(
    Interpreter     interpreter,
    CriaBoolean     boolean
);



#endif
