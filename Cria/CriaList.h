#ifndef PUBLIC_CRIALIST_H_INCLUDED
#define PUBLIC_CRIALIST_H_INCLUDED


#include "String.h"
#include "Interpreter.h"
#include "CriaId.h"
#include "Definition.h"
#include "List.h"



typedef struct CriaListTag *CriaList;



struct CriaListTag
{
    struct CriaIdTag id;
    List pointer;
};



DefinitionClass
CriaList_loadClass(
    char* name
);



#endif
