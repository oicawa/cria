#include "_Cria.h"



//==============================
//IO
//==============================
//標準出力へ文字列を出力。
//★問題点★
//可変長引数のフォーマットだと引数の数が取得できない為、
//フォーマット文字列中にある埋め込み文字の識別子？（%s）
//よりも引数が少なかった場合、Segmentation faultが発生してしまう。
//解決策あるのかな。。Listで渡す？最終的にはその方がいいかもなぁ。
CriaObject
io_print(
    Interpreter interpreter,
    List        args
)
{
    CriaObject  returnValue = CriaObject_new(NULL, CRIA_DATA_TYPE_VOID, NULL);
    CriaObject  criaObject = NULL;
    char*       start = NULL;
    char*       end = NULL;
    long        index = 0;
    long        count = args->count;
    
    //引数が０だった場合は実行時エラー
    if (count == 0)
    {
        Logger_err("Runtime error. (Argument count is 0.)");
        perror("Runtime error. (Argument count is 0.)");
        exit(1);
    }

    //第一引数がStringでなかった場合はエラー    
    criaObject = (CriaObject)list_get(args, 0);
    if (criaObject->type != CRIA_DATA_TYPE_STRING)
    {
        Logger_err("Runtime error. (First argument is not String.)");
        perror("Runtime error. (First argument is not String.)");
        exit(1);
    }
    
    //第一引数の文字列型データを抽出
    start = ((CriaString)(criaObject->object))->value->pointer;
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
        criaObject = (CriaObject)list_get(args, index);
        
        //NULLチェック
        if (criaObject == NULL)
        {
            printf("(null)");
            continue;
        }
        
        //型チェック
        if (criaObject->type != CRIA_DATA_TYPE_STRING)
        {
            Logger_err("Runtime error. (First argument is not String.)");
            perror("Runtime error. (First argument is not String.)");
            exit(1);
        }
        
        //出力
        printf("%s", ((CriaString)(criaObject->object))->value->pointer);
    }
    printf(start);
    
    
    return returnValue;
}




