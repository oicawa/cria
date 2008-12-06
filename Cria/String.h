#ifndef PUBLIC_STRING_H_INCLUDED
#define PUBLIC_STRING_H_INCLUDED

#include "Boolean.h"


typedef char* String;

String
string_newFunction(
    char    *input,
    char    *fileName,
    int     line
);



void
string_dispose(
    String  string
);



long
string_length(
    String  string
);



String
string_cloneFunction(
    String  source,
    char    *fileName,
    int     line
);



String
string_subStringFunction(
    String  source,
    long    start,
    long    length,
    char    *fileName,
    int     line
);



int
string_toInteger(
    String  source
);



Boolean
string_toBoolean(
    String  source
);



#endif
