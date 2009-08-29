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
#include "List.h"
#include "CriaObject.h"
#include "CriaBoolean.h"
#include "CriaInteger.h"
#include "CriaList.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"
#include "Definition.h"

#include "CriaString.h"


CriaString
CriaString_new(
    Boolean         isLiteral,
    String          value
)
{
    Logger_trc("[ START ]%s(%s)", __func__, value);
    CriaString criaString = Memory_malloc(sizeof(struct CriaStringTag));
    
    criaString->id.name = String_new("String");
    criaString->id.type = CRIA_DATA_TYPE_STRING;
    criaString->isLiteral = isLiteral;
    criaString->value = String_new(value);
    
    Logger_trc("[  END  ]%s", __func__);
    return criaString;
}



CriaId
CriaString_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaString      left,
    CriaString      right
)
{
    String leftValue = left->value;
    String rightValue = right->value;
    StringBuffer buffer = NULL;
    Boolean result = FALSE;
    CriaId id = NULL;
    
    switch (kind)
    {
    case OPERATION_KIND_PLUS:
        buffer = StringBuffer_new();
        StringBuffer_append(buffer, leftValue);
        StringBuffer_append(buffer, rightValue);
        id = (CriaId)CriaString_new(FALSE, StringBuffer_toString(buffer));
        break;
    case OPERATION_KIND_EQUAL:
        if (strcmp(leftValue, rightValue) == 0)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (strcmp(leftValue, rightValue) != 0)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    default:
        Runtime_error(interpreter, "Not supported operation.");
        break;
    }
    
    
    return id;
}



String
CriaString__core_(
    Interpreter interpreter,
	CriaId object
)
{
    Logger_trc("[ START ]%s", __func__);
    String value = NULL;
    
    if (object->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "Data type is not String.");
    	goto END;
    }
    
    value = ((CriaString)object)->value;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



CriaId
CriaString_sub(
    Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    String value = NULL;
    String sub = NULL;
    CriaId id = NULL;
    CriaId tmp = NULL;
    long start = 0;
    long length = 0;
    int count = 0;
    
    count = List_count(args);
    
    if (count < 1 || 2 < count)
    {
    	Runtime_error(interpreter, "Illegal argument count.");
    	goto END;
    }
    
    
    value = CriaString__core_(interpreter, object);
    length = strlen(value);
    
    
    tmp = (CriaId)(List_get(args, 0));
    if (tmp->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Runtime_error(interpreter, "1st argument data type is not Integer.");
    	goto END;
    }
    start = ((CriaInteger)tmp)->value;
    
    if (count == 2)
    {
        tmp = (CriaId)(List_get(args, 1));
        if (tmp->type != CRIA_DATA_TYPE_INTEGER)
        {
           	Runtime_error(interpreter, "2st argument data type is not Integer.");
        	goto END;
        }
        length = ((CriaInteger)tmp)->value;
    }
    else
    {
        length = String_length(value) - start;
    }
    
    
    sub = String_sub(value, start, length);
    
    id = (CriaId)CriaString_new(FALSE, sub);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaString_find(
    Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    String value = NULL;
    String string = NULL;
    CriaId id = NULL;
    CriaId tmp = NULL;
    long index = 0;
    long offset = 0;
    int count = 0;
    
    count = List_count(args);
    
    if (count < 1 || 2 < count)
    {
    	Runtime_error(interpreter, "Illegal argument count.");
    	goto END;
    }
    
    
    value = CriaString__core_(interpreter, object);
    
    
    tmp = (CriaId)(List_get(args, 0));
    if (tmp->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "1st argument data type is not String.");
    	goto END;
    }
    string = ((CriaString)tmp)->value;
    
    if (count == 2)
    {
        tmp = (CriaId)(List_get(args, 1));
        if (tmp->type != CRIA_DATA_TYPE_INTEGER)
        {
            Runtime_error(interpreter, "2st argument data type is not Integer.");
        	goto END;
        }
        offset = ((CriaInteger)tmp)->value;
    }
    else
    {
        offset = 0;
    }
    
    
    index = String_find(value, string, offset);
    
    id = (CriaId)CriaInteger_new(FALSE, index);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaString_split(
    Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    String value = NULL;
    String sub = NULL;
    String separator = NULL;
    List list = NULL;
    CriaId id = NULL;
    CriaId tmp = NULL;
    long index = 0;
    long offset = 0;
    int count = 0;
    long length = 0;
    
    count = List_count(args);
    
    if (count != 1)
    {
    	Runtime_error(interpreter, "Illegal argument count.");
    	goto END;
    }
    
    
    value = CriaString__core_(interpreter, object);
    
    
    tmp = (CriaId)(List_get(args, 0));
    if (tmp->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "1st argument data type is not String.");
    	goto END;
    }
    separator = ((CriaString)tmp)->value;
    length = strlen(separator);
    
    
    list = List_new();
    while (TRUE)
    {
        index = String_find(value, separator, offset);
        if (index < 0)
        {
            sub = String_sub(value, offset, strlen(value) - offset);
            List_add(list, CriaString_new(FALSE, sub));
            break;
        }
        sub = String_sub(value, offset, index - offset);
        List_add(list, CriaString_new(FALSE, sub));
        offset = index + length;
    }
    
    id = CriaList__generator_(interpreter, NULL, NULL, NULL);
    CriaObject_set(interpreter, (CriaObject)id, "pointer", list);

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



DefinitionClass
CriaString_loadClass(
    String className
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    Hash i_fields = Hash_new(32);
    Hash s_fields = Hash_new(32);
    Hash i_methods = Hash_new(32);
    Hash s_methods = Hash_new(32);
    
    function = DefinitionFunction_new("sub", TRUE, FALSE, NULL, NULL, CriaString_sub);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("find", TRUE, FALSE, NULL, NULL, CriaString_find);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("split", TRUE, FALSE, NULL, NULL, CriaString_split);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}

