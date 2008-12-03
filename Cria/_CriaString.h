#ifndef PRIVATE_CRIASTRING_H_INCLUDED
#define PRIVATE_CRIASTRING_H_INCLUDED


#include "_Boolean.h"
#include "_CriaId.h"

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



void
CriaString_dispose(
    CriaString criaString
);



#endif
