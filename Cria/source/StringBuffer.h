#ifndef PUBLIC_STRINGBUFFER_H_INCLUDED
#define PUBLIC_STRINGBUFFER_H_INCLUDED


#include "Cria.h"
#include "List.h"
#include "String.h"


#define STRING_BUFFER_SIZE  256



StringBuffer
StringBuffer_new(
);



void
StringBuffer_appendChar(
    StringBuffer    stringBuffer,
    char            charactor
);



void
StringBuffer_append(
    StringBuffer    stringBuffer,
    char*           string
);



String
StringBuffer_toString(
    StringBuffer    stringBuffer
);



#endif

