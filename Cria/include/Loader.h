#ifndef PUBLIC_LOADER_H_INCLUDED
#define PUBLIC_LOADER_H_INCLUDED


#include "Parser.h"


typedef struct LoaderTag *Loader;



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



#endif
