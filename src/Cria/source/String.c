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
    char *input
)
{
	Logger_cor("[ START ]%s", __func__);
    char* string = NULL;
    
    if (input == NULL)
        return NULL;
    
    string = Memory_malloc(strlen(input) + 1);
    strcpy(string, input);
	Logger_cor("[  END  ]%s", __func__);
    return (String)string;
}



long
String_length(
    String  string
)
{
    return strlen(string);
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
    
    long size = strlen(source);
    
    if (start < 0 || length < 0)
        return NULL;
    
    if (size < start + length)
        return NULL;
    
    char* buffer = Memory_malloc(length + 1);
    strncpy(buffer, &(source[start]), length);
    
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
    char* pointer = NULL;
    
    if (source == NULL)
        return -1;
    
    if (source == NULL)
        return -1;
    
    pointer = strstr(&(source[offset]), string);
    if (pointer == NULL)
        return -1;

    index = pointer - source;
    
    return index;
}



int
String_toInteger(
    String  source
)
{
    return atoi(source);
}


Boolean
String_toBoolean(
    String  source
)
{
    Boolean boolean = -1;
    
    if (strcmp(source, "true") == 0)
    {
        boolean = TRUE;
    }
    else if (strcmp(source, "false") == 0)
    {
        boolean = FALSE;
    }
    
    return boolean;
}


