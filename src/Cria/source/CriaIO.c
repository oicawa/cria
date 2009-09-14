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

 
#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "CriaInteger.h"
#include "CriaString.h"
#include "CriaBoolean.h"
#include "StringBuffer.h"
#include "Runtime.h"
#include "CriaIO.h"


#define BUFFER_SIZE 5


CriaId
CriaIO_write(
    Interpreter interpreter,
    CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId returnId;
    returnId = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    
    
    CriaId id;
    wchar_t* start = NULL;
    wchar_t* end = NULL;
    long index = 0;
    int count = List_count(args);
    
    
    if (count == 0)
    {
        Runtime_error(interpreter, "Argument count error.");
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
        printf("%s", String_wcsrtombs(string->value));
        goto END;
    }
    
    if (id->type == CRIA_DATA_TYPE_STRING)
    {
        Logger_dbg("String");
        start = ((CriaString)id)->value;
        while((end = wcsstr(start, L"%s")) != NULL)
        {
            long size = end - start;
            wchar_t* buffer = Memory_malloc(size + 1);
            wcsncpy(buffer, start, size);
            printf(String_wcsrtombs(buffer));
            buffer = NULL;
            start = end + sizeof(wchar_t) * 2;
            
            if (count < index)
            {
                Runtime_error(interpreter, "Index is parameter count over.");
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
                printf("%s", String_wcsrtombs(((CriaString)id)->value));
                continue;
            }
            
            if (id->type == CRIA_DATA_TYPE_INTEGER)
            {
                printf("%d", ((CriaInteger)id)->value);
                continue;
            }
                        Runtime_error(interpreter, "Data type is not String, Integer, Boolean.");

            if (id->type == CRIA_DATA_TYPE_BOOLEAN)
            {
                CriaString string = (CriaString)CriaBoolean_toString(interpreter, (CriaBoolean)id);
                printf("%s", String_wcsrtombs(string->value));
                continue;
            }
            
        	Logger_err("id->type = %d", id->type);
            Runtime_error(interpreter, "Data type is not String, Integer, Boolean.");
        }
        Logger_dbg("[print]%s", start);
        printf(String_wcsrtombs(start));
    }
    else
    {
        Runtime_error(interpreter, "Data type is not String, Integer, Boolean.");
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
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString  returnString = NULL;
    StringBuffer stringBuffer = NULL;
    char buffer[BUFFER_SIZE];
    
    if (List_count(args) != 0)
    {
        Logger_err("Runtime error. (Argument count is not 0.)");
        Runtime_error(interpreter, "Argument count is not 0.");
        goto END;
    }
    
    
    stringBuffer = StringBuffer_new();
    
    Memory_reset(buffer, sizeof(buffer));
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        Logger_dbg("input = '%s'", buffer);
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
            StringBuffer_append(stringBuffer, String_mbsrtowcs(buffer));
            break;
        }
        StringBuffer_append(stringBuffer, String_mbsrtowcs(buffer));
        Memory_reset(buffer, sizeof(buffer));
    }
    
    returnString = CriaString_new(TRUE, StringBuffer_toString(stringBuffer));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)returnString;
}




