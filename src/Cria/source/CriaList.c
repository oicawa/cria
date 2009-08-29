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
#include "CriaBoolean.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"
#include "CriaObject.h"
#include "CriaInteger.h"
#include "Definition.h"
#include "CriaList.h"


CriaId
CriaList__generator_(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject list = CriaObject_new("List");
    CriaObject_addField(list, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "pointer", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)list;
}



List
CriaList__core_(
	Interpreter interpreter,
	CriaId object
)
{
    Logger_trc("[ START ]%s", __func__);
    List list = NULL;

    
    Logger_dbg("Check object data type.");
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }

    if (strcmp(object->name, "List") != 0)
    {
    	Runtime_error(interpreter, "Data type name of scoped object is not 'List'.");
    	goto END;
    }

    list = (List)CriaObject_get(interpreter, (CriaObject)object, "pointer");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return list;
}



CriaId
CriaList_new(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject list = NULL;
    int count = 0;
    int i = 0;
    CriaId item = NULL;
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    
    
    list = (CriaObject)object;
    List pointer = List_new();
    CriaObject_set(interpreter, list, "pointer", pointer);

    
    count = List_count(args);
    
    for (i = 0; i < count; i++)
    {
        item = List_get(args, i);
        List_add(pointer, item);
    }
    

END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)list;
}



CriaId
CriaList_add(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId arg = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    arg = (CriaId)(List_get(args, 0));
    
    
    List_add(pointer, arg);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_delete(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId arg = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    arg = (CriaId)(List_get(args, 0));
    if (arg->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Runtime_error(interpreter, "Data type of 1st argument is not Integer.");
    	goto END;
    }
    
    
    List_delete(pointer, ((CriaInteger)arg)->value);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_insert(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId index = NULL;
    CriaId target = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 2)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    index = (CriaId)(List_get(args, 0));
    if (index->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Runtime_error(interpreter, "Data type of 1st argument is not Integer.");
    	goto END;
    }
    
    target = (CriaId)(List_get(args, 1));
    
    List_insert(pointer, ((CriaInteger)index)->value, target);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_get(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId index = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    index = (CriaId)(List_get(args, 0));
    if (index->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Runtime_error(interpreter, "Data type of 1st argument is not Integer.");
    	goto END;
    }
    
    
    id = (CriaId)List_get(pointer, ((CriaInteger)index)->value);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_set(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId index = NULL;
    void* value = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 2)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    index = (CriaId)(List_get(args, 0));
    if (index->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Runtime_error(interpreter, "Data type of 1st argument is not Integer.");
    	goto END;
    }
    
    value = (List_get(args, 1));
    
    List_set(pointer, ((CriaInteger)index)->value, value);

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_count(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    id = (CriaId)CriaInteger_new(FALSE, List_count(pointer));

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_each(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    if (block == NULL)
    {
    	Runtime_error(interpreter, "'block' is not defined.");
    	goto END;
    }
    
    
    int count = List_count(pointer);
    int i = 0;
    CriaId item = NULL;
    for (i = 0; i < count; i++)
    {
        item = (CriaId)List_get(pointer, i);
        List parameters = List_new();
        List_add(parameters, item);
        CriaBlock_evaluate(block, parameters);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



DefinitionClass
CriaList_loadClass(
    String className
)
{
    Logger_trc("[ START ]%s", __func__);
//    DefinitionVariable variable = NULL;
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    Hash i_fields = Hash_new(32);
    Hash s_fields = Hash_new(32);
    Hash i_methods = Hash_new(32);
    Hash s_methods = Hash_new(32);
    
    //variable = DefinitionVariable_new("file", FALSE, FALSE);
    //Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaList__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaList_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("add", TRUE, FALSE, NULL, NULL, CriaList_add);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("delete", TRUE, FALSE, NULL, NULL, CriaList_delete);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("insert", TRUE, FALSE, NULL, NULL, CriaList_insert);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("get", TRUE, FALSE, NULL, NULL, CriaList_get);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("get[]", TRUE, FALSE, NULL, NULL, CriaList_get);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);

    function = DefinitionFunction_new("set[]", TRUE, FALSE, NULL, NULL, CriaList_set);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("count", TRUE, FALSE, NULL, NULL, CriaList_count);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("each", TRUE, FALSE, NULL, NULL, CriaList_each);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



