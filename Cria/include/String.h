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


long
String_length(
    String  string
);



String
String_cloneFunction(
    String  source,
    char    *fileName,
    int     line
);
#define String_clone(source)\
	String_cloneFunction(source, __FILE__, __LINE__)



String
String_sub(
    String  source,
    long    start,
    long    length
);



long
String_find(
    String source,
    String string,
    long offset
);



int
String_toInteger(
    String  source
);



Boolean
String_toBoolean(
    String  source
);



#endif
