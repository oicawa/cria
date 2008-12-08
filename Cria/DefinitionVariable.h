#ifndef PUBLIC_DEFINITIONVARIABLE_H_INCLUDED
#define PUBLIC_DEFINITIONVARIABLE_H_INCLUDED



#include "String.h"
#include "List.h"
#include "CriaId.h"



typedef struct  DefinitionVariableTag *DefinitionVariable;



DefinitionVariable
definition_variable_new(
    String  name
);



DefinitionVariable
definition_variable_search(
    List parameters,
    String name
);



String
definition_variable_name(
	DefinitionVariable variable
);



void
definition_variable_set(
	DefinitionVariable variable,
	CriaId id
);



CriaId
DefinitionVariable_getObject(
	DefinitionVariable variable
);



#endif
