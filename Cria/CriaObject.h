#ifndef PUBLIC_CRIAOBJECT_H_INCLUDED
#define PUBLIC_CRIAOBJECT_H_INCLUDED



#include "CriaId.h"
#include "List.h"
#include "Definition.h"



typedef struct CriaObjectTag *CriaObject;



struct CriaObjectTag
{
    struct CriaIdTag    id;
    List fields;
};



CriaObject
CriaObject_new(
	String className
);



void
CriaObject_addField(
	CriaObject object,
    DefinitionVariable variable
);



void*
CriaObject_get(
	Interpreter interpreter,
	CriaObject object,
    String name
);



void
CriaObject_set(
	Interpreter interpreter,
	CriaObject object,
    String name,
    void* value
);



#endif
