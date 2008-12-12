#include <string.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"


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
    
    string = Memory_mallocAt(fileName, line, strlen(input) + 1);
    memset(string, 0x00, strlen(input) + 1);
    strcpy(string, input);
	Logger_cor("[  END  ]%s", __func__);
    return (String)string;
}



void
String_dispose(
    String  string
)
{
    Logger_cor("[ START ]%s", __func__);
    if (string == NULL)
    {
        Logger_cor("string is NULL.");
        goto END;
    }
    
	Logger_cor("Free string.");
	Memory_free(string);
	Logger_cor("Set NULL to string.");
	string = NULL;
    
END:
    Logger_cor("[  END  ]%s", __func__);
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
    char* buffer = Memory_mallocAt(fileName, line, size + 1);
    memset(buffer, 0x00, size + 1);
    strncpy(buffer, source, size);
    
    String string = String_newFunction(buffer, fileName, line);
    Memory_free(buffer);
    buffer = NULL;
    return string;
}



String
String_subStringFunction(
    String  source,
    long    start,
    long    length,
    char    *fileName,
    int     line
)
{
    if (source == NULL)
        return NULL;
    
    long size = strlen(source);
    
    if (size < start + length)
        return NULL;
    
    char* buffer = Memory_mallocAt(fileName, line, length + 1);
    memset(buffer, 0x00, length + 1);
    strncpy(buffer, &(source[start]), length);
    
    String string = String_newFunction(buffer, fileName, line);
    Memory_free(buffer);
    buffer = NULL;
    return string;
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


