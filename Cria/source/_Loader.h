#ifndef PRIVATE_LOADER_H_INCLUDED
#define PRIVATE_LOADER_H_INCLUDED

#include "Cria.h"
#include "List.h"
#include "Hash.h"

#include "Loader.h"


#define MAX_PATH_LENGTH 1024

struct LoaderTag
{
    String package_name;
    String library_name;
    void* library;
};



#endif
