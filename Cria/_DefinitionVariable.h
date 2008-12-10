#ifndef PRIVATE_DEFINITIONVARIABLE_H_INCLUDED
#define PRIVATE_DEFINITIONVARIABLE_H_INCLUDED



#include "Boolean.h"
#include "String.h"
#include "List.h"
#include "CriaId.h"

#include "DefinitionVariable.h"


struct DefinitionVariableTag
{
    String      name;
    Boolean     isStatic;
    CriaId      object;
};



DefinitionVariable
DefinitionVariable_new(
    String  name
);



DefinitionVariable
DefinitionVariable_search(
    List parameters,
    String name
);



#endif
