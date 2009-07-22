#ifndef PUBLIC_CRIACLASS_H_INCLUDED
#define PUBLIC_CRIACLASS_H_INCLUDED



#include "Cria.h"
#include "CriaId.h"
#include "List.h"
#include "Hash.h"
#include "Definition.h"



typedef struct CriaClassTag *CriaClass;



struct CriaClassTag
{
    struct CriaIdTag id;
    DefinitionClass definition;
};



CriaClass
CriaClass_new(
	DefinitionClass definition
);



void*
CriaClass_get(
	Interpreter interpreter,
	CriaClass klass,
    String variableName
);



DefinitionVariable
CriaClass_getVariable(
    Interpreter interpreter,
    CriaClass klass,
    String variableName
);



#endif
