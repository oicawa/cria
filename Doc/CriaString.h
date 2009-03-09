#ifndef PUBLIC_CRIASTRING_H_INCLUDED
#define PUBLIC_CRIASTRING_H_INCLUDED



#include "CriaId.h"
#include "Boolean.h"
#include "String.h"
#include "Interpreter.h"
#include "Expression.h"



typedef struct CriaStringTag
{
    struct CriaIdTag    id;
    Boolean             isLiteral;
    String              value;
} *CriaString;



CriaString
CriaString_new(
    Boolean         isLiteral,
    String          value
);



CriaId
CriaString_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaString      left,
    CriaString      right
);



#endif