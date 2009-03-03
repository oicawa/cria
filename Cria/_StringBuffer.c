#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"


#include "List.h"
#include "String.h"

#include "_StringBuffer.h"


#define STRING_BUFFER_SIZE  256



StringBuffer
StringBuffer_newFunction(
    char    *fileName,
    int     line
)
{
    Logger_cor("[ START ]%s", __func__);
    List list = List_new();
    Logger_cor("[  END  ]%s", __func__);
    return (StringBuffer)list;
}



void
StringBuffer_appendFunction(
    StringBuffer    stringBuffer,
    char*           string,
    char*           fileName,
    int             line
)
{
    Logger_cor("[ START ]%s", __func__);
    List    list = (List)stringBuffer;
    char*   buffer = NULL;
    Item    lastItem = NULL;
    long    length = 0;
    long    filler = 0;
    
    
    lastItem = List_lastItem(list);
    Logger_cor("lastItem is [%p]", lastItem);
    if (lastItem == NULL)
    {
	    Logger_cor("lastItem is NULL");
        buffer = Memory_malloc(STRING_BUFFER_SIZE);
        memset(buffer, 0x00, STRING_BUFFER_SIZE);
        List_addFunction(list, buffer, fileName, line);
        lastItem = List_lastItem(list);
    }
    
    
    Logger_cor("Get lastItem object");
    buffer = (char*)(Item_getObject(lastItem));
    Logger_cor("Got lastItem object");
    
    length = strlen(string);
    Logger_cor("string length = %d", length);
    
    filler = STRING_BUFFER_SIZE - strlen(buffer) - 1;
    Logger_cor("filter = %d", filler);
    
    strncat(buffer, string, filler);
    
    if (length <= filler)
    {
        return;
    }
    
    
    Logger_cor("Length is over.");
    buffer = Memory_malloc(STRING_BUFFER_SIZE);
    memset(buffer, 0x00, STRING_BUFFER_SIZE);
    List_addFunction(list, buffer, fileName, line);
    StringBuffer_append(stringBuffer, &(string[filler]));
    Logger_cor("[  END  ]%s", __func__);

}



void
StringBuffer_appendCharFunction(
    StringBuffer    stringBuffer,
    char            charactor,
    char*           fileName,
    int             line
)
{
    Logger_cor("[ START ]%s", __func__);
    char buffer[1 + 1];
    memset(buffer, 0x00, sizeof(buffer));
    sprintf(buffer, "%c", charactor);
    Logger_cor("buffer = [%s]", buffer);
    StringBuffer_append(stringBuffer, buffer);
    Logger_cor("[  END  ]%s", __func__);
}



String
StringBuffer_toStringFunction(
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
    
    item = List_startItem(list);
    Logger_cor("item set. (%p)", item);
    while(item != NULL)
    {
        Logger_cor("stringBuffer = [%s]", (char*)(Item_getObject(item)));
        length = length + strlen((char*)(Item_getObject(item)));
        item = Item_getNext(item);
    }
    
    
    Logger_cor("buffer set.");
    char* buffer = Memory_malloc(length + 1);
    memset(buffer, 0x00, length + 1);
    
    
    Logger_cor("start concat.");
    item = List_startItem(list);
    while(item != NULL)
    {
        strcat(buffer, (char*)(Item_getObject(item)));
        item = Item_getNext(item);
    }
    Logger_cor("stringBuffer all = [%s]", buffer);
    
    
    Logger_cor("Create string value.");
    value = String_new(buffer);
    buffer = NULL;
    
    
END:
    Logger_cor("[  END  ]%s", __func__);
    return value;
}



