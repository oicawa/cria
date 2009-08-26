#ifndef PUBLIC_CRIASTRING_H_INCLUDED
#define PUBLIC_CRIASTRING_H_INCLUDED



#include "Cria.h"
#include "CriaId.h"
#include "String.h"
#include "Interpreter.h"
#include "Expression.h"
#include "Definition.h"



CriaId
CriaString_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaString      left,
    CriaString      right
);


DefinitionClass
CriaString_loadClass(
    String className
);



#endif
