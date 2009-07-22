#ifndef PUBLIC_CRIABOOLEAN_H_INCLUDED
#define PUBLIC_CRIABOOLEAN_H_INCLUDED



#include "Cria.h"
#include "CriaId.h"
#include "Boolean.h"
#include "Interpreter.h"
#include "Expression.h"



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
