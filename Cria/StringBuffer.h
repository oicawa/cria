#ifndef PUBLIC_STRINGBUFFER_H_INCLUDED
#define PUBLIC_STRINGBUFFER_H_INCLUDED


#include "List.h"
#include "String.h"


typedef struct  ListTag *StringBuffer;


StringBuffer
StringBuffer_newFunction(
    char    *fileName,
    int     line
);
#define StringBuffer_new()\
	StringBuffer_newFunction(__FILE__, __LINE__);



void
stringBuffer_dispose(
    StringBuffer    stringBuffer
);



void
stringBuffer_appendCharFunction(
    StringBuffer    stringBuffer,
    char            charactor,
    char*           fileName,
    int             line
);
#define stringBuffer_appendChar(buffer, charctor)\
    (stringBuffer_appendCharFunction(buffer, charctor, __FILE__, __LINE__))



void
stringBuffer_appendFunction(
    StringBuffer    stringBuffer,
    char*           string,
    char*           fileName,
    int             line
);
#define stringBuffer_append(buffer, string)\
    (stringBuffer_appendFunction(buffer, string, __FILE__, __LINE__))



String
stringBuffer_toStringFunction(
    StringBuffer    stringBuffer,
    char*           fileName,
    int             line
);
#define stringBuffer_toString(buffer)\
    (stringBuffer_toStringFunction(buffer, __FILE__, __LINE__))



#endif

