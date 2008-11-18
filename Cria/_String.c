#include "_Cria.h"



//Stringを生成
String
string_newFunction(
    char    *input,
    char    *fileName,
    int     line
)
{
    String  string;
    
    if (input == NULL)
        return NULL;
    
    string = Memory_mallocAt(fileName, line, sizeof(struct StringTag));
    memset(string, 0x00, sizeof(struct StringTag));
    string->pointer = Memory_mallocAt(fileName, line, strlen(input) + 1);
    memset(string->pointer, 0x00, strlen(input) + 1);
    strcpy(string->pointer, input);
    return string;
}



//文字列を破棄
void
string_dispose(
    String  string
)
{
    Logger_cor("[ START ]%s", __func__);
    if (string == NULL)
    {
        Logger_cor("string is NULL.");
        goto END;
    }
    
    Logger_cor("Check string->pointer.");
    if (string->pointer != NULL)
    {
        Logger_cor("Free string->pointer.");
        Memory_free(string->pointer);
        Logger_cor("Set NULL to string->pointer.");
        string->pointer = NULL;
    }
    
    Logger_cor("Free string.");
    Memory_free(string);
    Logger_cor("Set NULL to string.");
    string = NULL;
    
END:
    Logger_cor("[  END  ]%s", __func__);
}



//文字列長を取得
long
string_length(
    String  string
)
{
    return strlen(string->pointer);
}



String
string_cloneFunction(
    String  source,
    char    *fileName,
    int     line
)
{
    if (source == NULL)
        return NULL;

    long size = strlen(source->pointer);
    char* buffer = Memory_mallocAt(fileName, line, size + 1);
    memset(buffer, 0x00, size + 1);
    strncpy(buffer, source->pointer, size);
    
    String string = string_newFunction(buffer, fileName, line);
    Memory_free(buffer);
    buffer = NULL;
    return string;
}



String
string_subStringFunction(
    String  source,
    long    start,
    long    length,
    char    *fileName,
    int     line
)
{
    if (source == NULL)
        return NULL;
    
    long size = strlen(source->pointer);
    
    if (size < start + length)
        return NULL;
    
    char* buffer = Memory_mallocAt(fileName, line, length + 1);
    memset(buffer, 0x00, length + 1);
    strncpy(buffer, &(source->pointer[start]), length);
    
    String string = string_newFunction(buffer, fileName, line);
    Memory_free(buffer);
    buffer = NULL;
    return string;
}



int
string_toInteger(
    String  source
)
{
    return atoi(source->pointer);
}


Boolean
string_toBoolean(
    String  source
)
{
    Boolean boolean = -1;
    
    if (strcmp(source->pointer, "true") == 0)
    {
        boolean = TRUE;
    }
    else if (strcmp(source->pointer, "false") == 0)
    {
        boolean = FALSE;
    }
    
    return boolean;
}


