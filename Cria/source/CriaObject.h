#ifndef PUBLIC_CRIAOBJECT_H_INCLUDED
#define PUBLIC_CRIAOBJECT_H_INCLUDED



#include "Cria.h"
#include "CriaId.h"
#include "List.h"
#include "Hash.h"
#include "Definition.h"






struct CriaObjectTag
{
    struct CriaIdTag id;
    Hash fields;
};



CriaObject
CriaObject_getObject(
	CriaId id,
    String class_name
);



#endif
