/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
#include <string.h>

#include "Cria.h"
#include "Memory.h"
#include "Logger.h"


#include "List.h"
#include "String.h"

#include "StringBuffer.h"


StringBuffer
StringBuffer_new(
)
{
    Logger_cor("[ START ]%s", __func__);
    List list = List_new();
    Logger_cor("[  END  ]%s", __func__);
    return (StringBuffer)list;
}



void
StringBuffer_append(
    StringBuffer    stringBuffer,
    String          string
)
{
    Logger_cor("[ START ]%s", __func__);
    List    list = (List)stringBuffer;
    String  buffer = NULL;
    Item    lastItem = NULL;
    long    length = 0;
    long    filler = 0;
    
    
    lastItem = List_lastItem(list);
    Logger_cor("lastItem is [%p]", lastItem);
    if (lastItem == NULL)
    {
	    Logger_cor("lastItem is NULL");
        buffer = Memory_malloc(sizeof(wchar_t) * STRING_BUFFER_SIZE);
        List_add(list, buffer);
        lastItem = List_lastItem(list);
    }
    
    
    Logger_cor("Get lastItem object");
    buffer = (String)(Item_getObject(lastItem));
    Logger_cor("Got lastItem object");
    
    length = wcslen(string);
    Logger_cor("string length = %d", length);
    
    filler = STRING_BUFFER_SIZE - wcslen(buffer) - 1;
    Logger_cor("filter = %d", filler);
    
    wcsncat(buffer, string, filler);
    
    if (length <= filler)
    {
        return;
    }
    
    
    Logger_cor("Length is over.");
    buffer = Memory_malloc(sizeof(wchar_t) * STRING_BUFFER_SIZE);
    List_add(list, buffer);
    StringBuffer_append(stringBuffer, &(string[filler]));
    Logger_cor("[  END  ]%s", __func__);

}



void
StringBuffer_appendChar(
    StringBuffer    stringBuffer,
    wchar_t         charactor
)
{
    Logger_cor("[ START ]%s", __func__);
    wchar_t buffer[1 + 1];
    Memory_reset(buffer, sizeof(buffer));
    buffer[0] = charactor;
    StringBuffer_append(stringBuffer, buffer);
    Logger_cor("[  END  ]%s", __func__);
}



String
StringBuffer_toString(
    StringBuffer    stringBuffer
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
        length = length + wcslen((String)(Item_getObject(item)));
        item = Item_getNext(item);
    }
    
    
    Logger_cor("buffer set.");
    String buffer = Memory_malloc(sizeof(wchar_t) * (length + 1));
    
    
    Logger_cor("start concat.");
    item = List_startItem(list);
    while(item != NULL)
    {
        wcscat(buffer, (String)(Item_getObject(item)));
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



