#ifndef PRIVATE_DEFINITIONVARIABLE_H_INCLUDED
#define PRIVATE_DEFINITIONVARIABLE_H_INCLUDED



#include "_Boolean.h"
#include "_String.h"
#include "_List.h"
#include "_CriaId.h"



typedef struct  DefinitionVariableTag           *DefinitionVariable;
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
    List    variableList,
    String  name
);



#endif
