#ifndef PUBLIC_CRIAOBJECT_H_INCLUDED
#define PUBLIC_CRIAOBJECT_H_INCLUDED



#include "CriaId.h"
#include "List.h"



typedef struct CriaObjectTag *CriaObject;



struct CriaObjectTag
{
    struct CriaIdTag    id;
    List fields;
};



#endif
