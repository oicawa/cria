#ifndef PUBLIC_LOADER_H_INCLUDED
#define PUBLIC_LOADER_H_INCLUDED


#include "Cria.h"
#include "Definition.h"
#include "Parser.h"
#include "List.h"



#define MAX_PATH_LENGTH 1024

struct LoaderTag
{
    String package_name;
    String library_name;
    void* library;
};



Loader
Loader_new(
	List path
);



Loader
Loader_parse(
    Parser parser
);



String
Loader_get_package_name(
    Loader loader
);



void
Loader_load(
    Loader loader,
    Interpreter interpreter
);



void
Loader_add_function(
    Interpreter interpreter,
    char* functionName,
    CriaNativeFunction* functionPoint
);



void
Loader_add_class(
    Interpreter interpreter,
    char* className,
    CriaNativeClassLoader* classLoader
);



void
Loader_unload(
	Loader loader
);



#endif
