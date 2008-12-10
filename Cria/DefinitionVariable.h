#ifndef PUBLIC_DEFINITIONVARIABLE_H_INCLUDED
#define PUBLIC_DEFINITIONVARIABLE_H_INCLUDED



#include "String.h"
#include "List.h"
#include "CriaId.h"



typedef struct  DefinitionVariableTag *DefinitionVariable;



DefinitionVariable
DefinitionVariable_new(
    String  name
);



DefinitionVariable
DefinitionVariable_search(
    List parameters,
    String name
);



String
DefinitionVariable_name(
	DefinitionVariable variable
);



void
DefinitionVariable_set(
	DefinitionVariable variable,
	CriaId id
);



CriaId
DefinitionVariable_getObject(
	DefinitionVariable variable
);



#endif
