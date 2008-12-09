#ifndef PRIVATE_DEFINITIONCLASS_H_INCLUDED
#define PRIVATE_DEFINITIONCLASS_H_INCLUDED



#include "DefinitionClass.h"



struct DefinitionClassTag
{
    String      name;
    Boolean     isNative;
    List        baseList;
    List        fieldList;
    List        methodList;
};



#endif
