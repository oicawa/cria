#include <string.h>

#include "Memory.h"
#include "../Logger/Logger.h"

#include "Runtime.h"
#include "CriaInteger.h"
#include "CriaString.h"
#include "CriaBoolean.h"
#include "StringBuffer.h"

#include "_CriaIO.h"


#define BUFFER_SIZE 5


CriaId
CriaIO_write(
    Interpreter interpreter,
    CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId returnId;
    returnId = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    
    
    CriaId id;
    char*       start = NULL;
    char*       end = NULL;
    long        index = 0;
    int count = List_count(args);
    
    
    if (count == 0)
    {
        runtime_error(interpreter);
    }
    
    id = (CriaId)List_get(args, 0);
    
    
    if (id->type == CRIA_DATA_TYPE_INTEGER)
    {
        Logger_dbg("Integer");
        int value = ((CriaInteger)id)->value;
        printf("%d", value);
        goto END;
    }
    
    if (id->type == CRIA_DATA_TYPE_BOOLEAN)
    {
        Logger_dbg("Boolean");
        CriaString string = (CriaString)CriaBoolean_toString(interpreter, (CriaBoolean)id);
        printf("%s", string->value);
        goto END;
    }
    
    if (id->type == CRIA_DATA_TYPE_STRING)
    {
        Logger_dbg("String");
        start = ((CriaString)id)->value;
        while((end = strstr(start, "%s")) != NULL)
        {
            long size = end - start;
            char* buffer = Memory_malloc(size + 1);
            memset(buffer, 0x00, size + 1);
            strncpy(buffer, start, size);
            printf(buffer);
            buffer = NULL;
            start = end + 2;
            
            if (count < index)
            {
                runtime_error(interpreter);
            }
            
            index += 1;
            id = (CriaId)List_get(args, index);
            
            if (id == NULL)
            {
                printf("(null)");
                continue;
            }
            
            if (id->type == CRIA_DATA_TYPE_STRING)
            {
                Logger_dbg("[print]%p", ((CriaString)id));
                printf("%s", ((CriaString)id)->value);
                continue;
            }
            
            if (id->type == CRIA_DATA_TYPE_INTEGER)
            {
                printf("%d", ((CriaInteger)id)->value);
                continue;
            }
            
            if (id->type == CRIA_DATA_TYPE_BOOLEAN)
            {
                CriaString string = (CriaString)CriaBoolean_toString(interpreter, (CriaBoolean)id);
                printf("%s", string->value);
                continue;
            }
            
        	Logger_err("id->type = %d", id->type);
            runtime_error(interpreter);
        }
        Logger_dbg("[print]%s", start);
        printf(start);
    }
    else
    {
        runtime_error(interpreter);
    }
    
END:
    fflush(stdout);
    Logger_trc("[  END  ]%s", __func__);
    return returnId;
}



CriaId
CriaIO_read(
    Interpreter interpreter,
    CriaId object,
    List        args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString  returnString = NULL;
    StringBuffer stringBuffer = NULL;
    char buffer[BUFFER_SIZE];
    
    if (List_count(args) != 0)
    {
        Logger_err("Runtime error. (Argument count is not 0.)");
        runtime_error(interpreter);
        goto END;
    }
    
    
    stringBuffer = StringBuffer_new();
    
    memset(buffer, 0x00, sizeof(buffer));
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        Logger_dbg("input = '%s'", buffer);
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
            StringBuffer_append(stringBuffer, buffer);
            break;
        }
        StringBuffer_append(stringBuffer, buffer);
        memset(buffer, 0x00, sizeof(buffer));
    }
    
    returnString = CriaString_new(TRUE, StringBuffer_toString(stringBuffer));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)returnString;
}




