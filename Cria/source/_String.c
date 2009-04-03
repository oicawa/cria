#include <string.h>

#include "Memory.h"
#include "Logger.h"


#include "_String.h"



String
String_newFunction(
    char    *input,
    char    *fileName,
    int     line
)
{
	Logger_cor("[ START ]%s", __func__);
    char* string = NULL;
    
    if (input == NULL)
        return NULL;
    
    string = Memory_malloc(strlen(input) + 1);
    strcpy(string, input);
	Logger_cor("[  END  ]%s", __func__);
    return (String)string;
}



long
String_length(
    String  string
)
{
    return strlen(string);
}



String
String_cloneFunction(
    String  source,
    char    *fileName,
    int     line
)
{
    if (source == NULL)
        return NULL;

    long size = strlen(source);
    char* buffer = Memory_malloc(size + 1);
    strncpy(buffer, source, size);
    
    String string = String_newFunction(buffer, fileName, line);
    buffer = NULL;
    return string;
}



String
String_sub(
    String  source,
    long    start,
    long    length
)
{
    if (source == NULL)
        return NULL;
    
    long size = strlen(source);
    
    if (start < 0 || length < 0)
        return NULL;
    
    if (size < start + length)
        return NULL;
    
    char* buffer = Memory_malloc(length + 1);
    strncpy(buffer, &(source[start]), length);
    
    String string = String_new(buffer);
    buffer = NULL;
    return string;
}



long
String_find(
    String source,
    String string,
    long offset
)
{
    long index = 0;
    char* pointer = NULL;
    
    if (source == NULL)
        return -1;
    
    if (source == NULL)
        return -1;
    
    pointer = strstr(&(source[offset]), string);
    if (pointer == NULL)
        return -1;

    index = pointer - source;
    
    return index;
}



int
String_toInteger(
    String  source
)
{
    return atoi(source);
}


Boolean
String_toBoolean(
    String  source
)
{
    Boolean boolean = -1;
    
    if (strcmp(source, "true") == 0)
    {
        boolean = TRUE;
    }
    else if (strcmp(source, "false") == 0)
    {
        boolean = FALSE;
    }
    
    return boolean;
}


