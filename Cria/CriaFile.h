#ifndef PUBLIC_CRIAFILE_H_INCLUDED
#define PUBLIC_CRIAFILE_H_INCLUDED


#include "String.h"
#include "Interpreter.h"
#include "CriaId.h"
#include "DefinitionClass.h"



typedef struct CriaFileTag *CriaFile;



struct CriaFileTag
{
    struct CriaIdTag    id;
    String path;
    FILE* pointer;
};



DefinitionClass
CriaFile_loadClass(
    Interpreter interpreter,
    char* name
);



#endif
