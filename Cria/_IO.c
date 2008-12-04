#include <string.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_Runtime.h"
#include "_CriaInteger.h"
#include "_CriaString.h"
#include "_CriaBoolean.h"
#include "_StringBuffer.h"

#include "_IO.h"


#define BUFFER_SIZE 5


CriaId
io_write(
    Interpreter interpreter,
    CriaId object,
    List        args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId returnId;
    returnId = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    
    
    CriaId id;
    char*       start = NULL;
    char*       end = NULL;
    long        index = 0;
    int count = args->count;
    
    
    if (count == 0)
    {
        runtime_error(interpreter);
    }
    
    id = (CriaId)list_get(args, 0);
    
    
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
        printf("%s", string->value->pointer);
        CriaString_dispose(string);
        goto END;
    }
    
    if (id->type == CRIA_DATA_TYPE_STRING)
    {
        Logger_dbg("String");
        //第一引数の文字列型データを抽出
        start = ((CriaString)id)->value->pointer;
        while((end = strstr(start, "%s")) != NULL)
        {
            long size = end - start;
            char* buffer = Memory_malloc(size + 1);
            memset(buffer, 0x00, size + 1);
            strncpy(buffer, start, size);
            printf(buffer);
            Memory_free(buffer);
            buffer = NULL;
            start = end + 2;
            
            //引数の個数が埋め込みフィールド数よりすくなかった場合はエラー
            if (count < index)
            {
                runtime_error(interpreter);
            }
            
            index += 1;
            id = (CriaId)list_get(args, index);
            
            //NULLチェック
            if (id == NULL)
            {
                printf("(null)");
                continue;
            }
            
            //型チェック
            if (id->type == CRIA_DATA_TYPE_STRING)
            {
                Logger_dbg("[print]%s", ((CriaString)id)->value->pointer);
                printf("%s", ((CriaString)id)->value->pointer);
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
                printf("%s", string->value->pointer);
                CriaString_dispose(string);
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
io_read(
    Interpreter interpreter,
    CriaId object,
    List        args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString  returnString = NULL;
    StringBuffer stringBuffer = NULL;
    char buffer[BUFFER_SIZE];
    
    //引数が０でない場合はエラー
    if (args->count != 0)
    {
        Logger_err("Runtime error. (Argument count is not 0.)");
        runtime_error(interpreter);
        goto END;
    }
    
    
    stringBuffer = stringBuffer_new();
    
    //標準入力から文字列を取得
    memset(buffer, 0x00, sizeof(buffer));
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        Logger_dbg("input = '%s'", buffer);
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
            stringBuffer_append(stringBuffer, buffer);
            break;
        }
        stringBuffer_append(stringBuffer, buffer);
        memset(buffer, 0x00, sizeof(buffer));
    }
    
    returnString = CriaString_new(TRUE, stringBuffer_toString(stringBuffer));
    
END:
    stringBuffer_dispose(stringBuffer);
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)returnString;
}




