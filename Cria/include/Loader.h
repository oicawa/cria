#ifndef PUBLIC_LOADER_H_INCLUDED
#define PUBLIC_LOADER_H_INCLUDED


#include "Parser.h"
#include "List.h"


typedef struct LoaderTag *Loader;



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
