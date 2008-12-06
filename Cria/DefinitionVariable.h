#ifndef PUBLIC_DEFINITIONVARIABLE_H_INCLUDED
#define PUBLIC_DEFINITIONVARIABLE_H_INCLUDED



#include "String.h"
#include "List.h"



typedef struct  DefinitionVariableTag *DefinitionVariable;



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
