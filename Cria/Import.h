#ifndef PUBLIC_IMPORT_H_INCLUDED
#define PUBLIC_IMPORT_H_INCLUDED


#include "Parser.h"


typedef struct ImportTag *Import;



Import
Import_parse(
    Parser parser
);



String
Import_get_package_name(
    Import import
);



#endif
