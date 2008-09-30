#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>



#include "_Cria.h"



#define STRING_BUFFER_SIZE  256



StringBuffer
stringBuffer_newFunction(
    char    *fileName,
    int     line
)
{
    List list = list_new();
    return (StringBuffer)list;
}



//Listの破棄
void
stringBuffer_dispose(
    StringBuffer    stringBuffer
)
{
    Logger_cor("[ START ]%s", __func__);
    if (stringBuffer == NULL)
    {
        Logger_cor("stringBuffer is NULL.");
        return;
    }
    
    Logger_cor("stringBuffer is casted to List.");
    List list = (List)stringBuffer;
    
    
    Item item = NULL;
    item = list->item;
    Logger_cor("Loop start.");
    while(item != NULL)
    {
        if (item->object != NULL)
        {
            Logger_cor("Free item->object.");
            Memory_free(item->object);
            Logger_cor("Set NULL to item->object.");
            item->object = NULL;
        }
        Logger_cor("Set next item.");
        item = item->next;
    }
    Logger_cor("Loop end.");
    
    
    Logger_cor("Free list->item.");
    item_dispose(list->item);
    list->item = NULL;
    
    Logger_cor("Free list.");
    Memory_free(list);
    Logger_cor("Set NULL to list.");
    list = NULL;
    
    Logger_cor("Set NULL to stringBuffer.");
    stringBuffer = NULL;
    Logger_cor("[  END  ]%s", __func__);
}



//文字、文字列を連結
void
stringBuffer_appendFunction(
    StringBuffer    stringBuffer,
    char*           string,
    char*           fileName,
    int             line
)
{
    List    list = (List)stringBuffer;
    char*   buffer = NULL;
    Item*   lastItem = NULL;
    long    length = 0;
    long    filler = 0;
    
    
    //終端Itemに対する操作
    if (list->last == NULL)
    {
        buffer = Memory_mallocAt(fileName, line, STRING_BUFFER_SIZE);
        memset(buffer, 0x00, STRING_BUFFER_SIZE);
        list_addFunction(list, buffer, fileName, line);
    }
    
    
    lastItem = list->last;
    buffer = (char*)((*lastItem)->object);
    
    length = strlen(string);
    filler = STRING_BUFFER_SIZE - strlen(buffer) - 1;
    
    //あまっている部分に文字列をコピー
    strncat(buffer, string, filler);
    
    //全てコピー可能だった場合は終了。
    if (length <= filler)
    {
        return;
    }
    
    
    //不足していた場合は新規領域を確保した後、再帰処理
    Logger_cor("Length is over.");
    buffer = Memory_mallocAt(fileName, line, STRING_BUFFER_SIZE);
    memset(buffer, 0x00, STRING_BUFFER_SIZE);
    list_addFunction(list, buffer, fileName, line);
    stringBuffer_append(stringBuffer, &(string[filler]));

}
void
stringBuffer_appendCharFunction(
    StringBuffer    stringBuffer,
    char            charactor,
    char*           fileName,
    int             line
)
{
    char buffer[1 + 1];
    memset(buffer, 0x00, sizeof(buffer));
    sprintf(buffer, "%c", charactor);
    Logger_cor("buffer = [%s]", buffer);
    stringBuffer_append(stringBuffer, buffer);
}



//StringBufferが保持している文字列群を一つの文字列として返却
String
stringBuffer_toStringFunction(
    StringBuffer    stringBuffer,
    char*           fileName,
    int             line
)
{
    Logger_cor("[ START ]%s", __func__);
    Item    item = NULL;
    long    length = 0;
    String value = NULL;
    
    Logger_cor("NULL check.");
    if (stringBuffer == NULL)
    {
        Logger_cor("stringBuffer is NULL.");
        goto END;
    }
    
    
    Logger_cor("Cast to List.");
    List list = (List)stringBuffer;
    
    //全長を取得
    item = list->item;
    Logger_cor("item set. (%p)", item);
    while(item != NULL)
    {
        Logger_cor("stringBuffer = [%s]", (char*)(item->object));
        length = length + strlen((char*)(item->object));
        item = item->next;
    }
    
    
    //書き込みバッファを確保
    Logger_cor("buffer set.");
    char* buffer = Memory_mallocAt(fileName, line, length + 1);
    memset(buffer, 0x00, length + 1);
    
    
    //内容を連結。
    Logger_cor("start concat.");
    item = list->item;
    while(item != NULL)
    {
        strcat(buffer, (char*)(item->object));
        item = item->next;
    }
    Logger_cor("stringBuffer all = [%s]", buffer);
    
    
    //文字列を生成して返却
    Logger_cor("Create string value.");
    value = string_new(buffer);
    Memory_free(buffer);
    buffer = NULL;
    
    
END:
    Logger_cor("[  END  ]%s", __func__);
    return value;
}



