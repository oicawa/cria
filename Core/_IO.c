#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



#include "_Core.h"
#include "../Memory/Memory.h"
#include "../Logger/Logger.h"
#include "../Cria/Cria.h"



//==============================
//IO
//==============================
//標準出力へ文字列を出力。
//★問題点★
//可変長引数のフォーマットだと引数の数が取得できない為、
//フォーマット文字列中にある埋め込み文字の識別子？（%s）
//よりも引数が少なかった場合、Segmentation faultが発生してしまう。
//解決策あるのかな。。Listで渡す？最終的にはその方がいいかもなぁ。
void
IO_print(
    //Interpreter interpreter,
    String      format,
    List        args
)
{
    char*   start = format->pointer;
    char*   end = NULL;
    long    index = 0;
    long    count = list_count(args);
    
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
        
        
        Object object = list_get(args, index);
        index += 1;
        
        if (object == NULL)
        {
            printf("(null)");
            continue;
        }
        String string = Object_toString(object);
        printf("%s", string->pointer);
        String_dispose(string);
        string = NULL;
    }
    printf(start);
}




