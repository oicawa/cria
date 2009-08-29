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

#include "CriaId.h"
#include "String.h"
#include "List.h"
#include "Hash.h"
#include "Definition.h"
#include "Runtime.h"

#include "CriaObject.h"



CriaObject
CriaObject_new(
	String className
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    CriaObject object = NULL;
    
    object = Memory_malloc(sizeof(struct CriaObjectTag));
    object->id.name = String_new(className);
    object->id.type = CRIA_DATA_TYPE_CRIA_OBJECT;
    object->fields = Hash_new(16);
    
    Logger_trc("[  END  ]%s", __func__);
    return object;
}



CriaObject
CriaObject_getObject(
	CriaId id,
    String class_name
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject object = NULL;

    
    Logger_dbg("Check object data type.");
    if (id->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	goto END;
    }
    
    if (strcmp(id->name, class_name) != 0)
    {
    	goto END;
    }
    
    object = (CriaObject)id;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return object;
}



void
CriaObject_addField(
	CriaObject object,
    DefinitionVariable variable
)
{
    Logger_trc("[ START ]%s", __func__);
    
    Hash_put(object->fields, DefinitionVariable_name(variable), variable);
    
    Logger_trc("[  END  ]%s", __func__);
    return;
}



void*
CriaObject_get(
	Interpreter interpreter,
	CriaObject object,
    String name
)
{
    Logger_trc("[ START ]%s", __func__);
    void* value = NULL;
    DefinitionVariable variable = NULL;
    
    
    //variable = DefinitionVariable_search(object->fields, name);
    variable = (DefinitionVariable)Hash_get(object->fields, name);
    if (variable == NULL)
    {
    	Logger_err("Not found variable named '%s'", name);
    	goto END;
    }
    
    
    value = DefinitionVariable_getObject(variable);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



void
CriaObject_set(
	Interpreter interpreter,
	CriaObject object,
    String name,
    void* value
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable variable = NULL;
    
    Logger_dbg("variable name = '%s'", name);
    variable = (DefinitionVariable)Hash_get(object->fields, name);
    Logger_dbg("fields = '%p'", object->fields);
    if (variable == NULL)
    {
    	Runtime_error(interpreter, "Variable is NULL.");
    	goto END;
    }
    
    DefinitionVariable_set(variable, value);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}




