#ifndef PUBLIC_STRING_H_INCLUDED
#define PUBLIC_STRING_H_INCLUDED

#include "Cria.h"



String
String_new(
    char    *input
);


long
String_length(
    String  string
);


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
