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
definition_variable_new(
    String  name
);



DefinitionVariable
definition_variable_search(
    List parameters,
    String name
);



#endif
