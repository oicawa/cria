#include "_Cria.h"


#define BUFFER_SIZE 5


CriaId
io_write(
    Interpreter interpreter,
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
    
    //引数が０だった場合は実行時エラー
    if (count == 0)
    {
        Logger_err("Runtime error. (Argument count is 0.)");
        perror("Runtime error. (Argument count is 0.)");
        goto END;
    }
    
    //第一引数がStringでなかった場合はエラー    
    id = (CriaId)list_get(args, 0);
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        Logger_err("Runtime error. (First argument is not String.) [%d]", id->type);
        perror("Runtime error. (First argument is not String.)");
        goto END;
    }
    
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
            Logger_err("Runtime error. (Argument is too few.)");
            perror("Runtime error. (Argument is too few.)");
            exit(1);
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
        if (id->type != CRIA_DATA_TYPE_STRING)
        {
            Logger_err("Runtime error. (First argument is not String.)");
            perror("Runtime error. (First argument is not String.)");
            exit(1);
        }
        
        //出力
        printf("%s", ((CriaString)id)->value->pointer);
    }
    printf(start);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return returnId;
}



CriaId
io_read(
    Interpreter interpreter,
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
        perror("Runtime error. (Argument count is not 0.)");
        goto END;
    }
    
    
    stringBuffer = stringBuffer_new();
    
    //標準入力から文字列を取得
    memset(buffer, 0x00, sizeof(buffer));
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        Logger_dbg("input = '%s'", buffer);
        stringBuffer_append(stringBuffer, buffer);
        memset(buffer, 0x00, sizeof(buffer));
    }
    
    returnString = CriaString_new(NULL, TRUE, stringBuffer_toString(stringBuffer));
    
END:
    stringBuffer_dispose(stringBuffer);
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)returnString;
}




