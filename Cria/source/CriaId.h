#ifndef PUBLIC_CRIAID_H_INCLUDED
#define PUBLIC_CRIAID_H_INCLUDED


#include "Cria.h"
#include "String.h"






typedef struct CriaIdTag
{
    String          name;
    CriaDataType    type;
    int             refCount;
} *CriaId;



CriaId
CriaId_new(
    String          name,
    CriaDataType    type
);



#endif
