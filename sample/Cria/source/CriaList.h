#ifndef PUBLIC_CRIALIST_H_INCLUDED
#define PUBLIC_CRIALIST_H_INCLUDED


#include "Cria.h"
#include "String.h"
#include "Interpreter.h"
#include "CriaId.h"
#include "Definition.h"
#include "List.h"



DefinitionClass
CriaList_loadClass(
    char* name
);



CriaId
CriaList__generator_(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
);



List
CriaList__core_(
	Interpreter interpreter,
	CriaId object
);



#endif