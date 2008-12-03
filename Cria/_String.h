#ifndef PRIVATE_STRING_H_INCLUDED
#define PRIVATE_STRING_H_INCLUDED


#include "_Boolean.h"


typedef struct StringTag *String;
struct StringTag {
    char*       pointer;
};



String
string_newFunction(
    char    *input,
    char    *fileName,
    int     line
);
#define string_new(string)\
    (string_newFunction(string, __FILE__, __LINE__))



void
string_dispose(
    String  string
);



String
string_cloneFunction(
    String  source,
    char    *fileName,
    int     line
);
#define string_clone(source)\
    (string_cloneFunction(source, __FILE__, __LINE__))



int
string_toInteger(
    String  source
);



Boolean
string_toBoolean(
    String  source
);



#endif
