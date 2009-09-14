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

 
#include "Memory.h"
#include "Logger.h"
#include "Cria.h"


#include "String.h"



String
String_new(
    wchar_t *input
)
{
	Logger_cor("[ START ]%s", __func__);
    wchar_t* string = NULL;
    int length = wcslen(input);
    
    if (input == NULL)
        return NULL;
    
    string = Memory_malloc(sizeof(wchar_t) * (length + 1));
    wcsncpy(string, input, length);
	Logger_cor("[  END  ]%s", __func__);
    return (String)string;
}


String
String_mbsrtowcs(
    char* value
)
{
    wchar_t wcbuffer[256 + 1];
    const char* multi = NULL;
    mbstate_t ps;
    String string = NULL;
    
    memset(wcbuffer, 0x00, sizeof(wcbuffer));
    memset(&ps, 0x00, sizeof(mbstate_t));
    
    multi = value;
    if (mbsrtowcs(wcbuffer, &multi, sizeof(wcbuffer), &ps) < 0)
    {
        goto END;
    }
    
    string = String_new(wcbuffer);
END:
    return string;
}


char*
String_wcsrtombs(
    String value
)
{
    char buffer[256 + 1];
    const wchar_t* wide = value;
    mbstate_t ps;
    char* string = NULL;
    
    memset(buffer, 0x00, sizeof(buffer));
    memset(&ps, 0x00, sizeof(mbstate_t));
    
    if (wcsrtombs(buffer, &wide, sizeof(buffer), &ps) < 0)
    {
        goto END;
    }
    
    string = Memory_malloc(strlen(buffer) + 1);
    strncpy(string, buffer, strlen(buffer));
END:
    return string;
}


long
String_length(
    String  string
)
{
    return wcslen(string);
}



String
String_sub(
    String  source,
    long    start,
    long    length
)
{
    if (source == NULL)
        return NULL;
    
    long size = wcslen(source);
    
    if (start < 0 || length < 0)
        return NULL;
    
    if (size < start + length)
        return NULL;
    
    String buffer = Memory_malloc(sizeof(wchar_t) * (length + 1));
    wcsncpy(buffer, &(source[start]), length);
    
    String string = String_new(buffer);
    buffer = NULL;
    return string;
}



long
String_find(
    String source,
    String string,
    long offset
)
{
    long index = 0;
    String pointer = NULL;
    
    
    if (source == NULL)
        return -1;
    
    if (source == NULL)
        return -1;
    
    pointer = wcsstr(&(source[offset]), string);
    if (pointer == NULL)
        return -1;

    index = pointer - source;
    
    return index;
}



long
String_toInteger(
    String  source
)
{
    return wcstol(source, NULL, 10);
}


Boolean
String_toBoolean(
    String  source
)
{
    Boolean boolean = -1;
    
    if (wcscmp(source, L"true") == 0)
    {
        boolean = TRUE;
    }
    else if (wcscmp(source, L"false") == 0)
    {
        boolean = FALSE;
    }
    
    return boolean;
}


