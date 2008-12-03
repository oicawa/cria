#ifndef PRIVATE_CRIAFILE_H_INCLUDED
#define PRIVATE_CRIAFILE_H_INCLUDED



#include "_Cria.h"


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
