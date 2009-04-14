#ifndef PUBLIC_CRIAVARIABLE_H_INCLUDED
#define PUBLIC_CRIAVARIABLE_H_INCLUDED


#include "String.h"
#include "Interpreter.h"
#include "CriaId.h"
#include "Definition.h"
#include "List.h"



typedef struct CriaVariableTag *CriaVariable;



struct CriaVariableTag
{
    struct CriaIdTag id;
    DefinitionVariable definition;
};



CriaId
CriaVariable_new(
    String name,
	DefinitionVariable definition
);



#endif
