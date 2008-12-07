#ifndef PUBLIC_STRING_H_INCLUDED
#define PUBLIC_STRING_H_INCLUDED

#include "Boolean.h"


typedef char* String;

String
String_newFunction(
    char    *input,
    char    *fileName,
    int     line
);
#define String_new(input)\
	String_newFunction(input, __FILE__, __LINE__)


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
#define string_clone(source)\
	string_cloneFunction(source, __FILE__, __LINE__)



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
