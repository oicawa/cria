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
StringBuffer_dispose(
    StringBuffer    stringBuffer
);



void
StringBuffer_appendCharFunction(
    StringBuffer    stringBuffer,
    char            charactor,
    char*           fileName,
    int             line
);
#define StringBuffer_appendChar(buffer, charctor)\
    (StringBuffer_appendCharFunction(buffer, charctor, __FILE__, __LINE__))



void
StringBuffer_appendFunction(
    StringBuffer    stringBuffer,
    char*           string,
    char*           fileName,
    int             line
);
#define StringBuffer_append(buffer, string)\
    (StringBuffer_appendFunction(buffer, string, __FILE__, __LINE__))



String
StringBuffer_toStringFunction(
    StringBuffer    stringBuffer,
    char*           fileName,
    int             line
);
#define StringBuffer_toString(buffer)\
    (StringBuffer_toStringFunction(buffer, __FILE__, __LINE__))



#endif

