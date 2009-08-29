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
#include "Hash.h"
#include "Definition.h"
#include "Runtime.h"
#include "CriaClass.h"



CriaClass
CriaClass_new(
	DefinitionClass definition
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    CriaClass klass = NULL;
    
    klass = Memory_malloc(sizeof(struct CriaClassTag));
    klass->id.name = String_new("Class");
    klass->id.type = CRIA_DATA_TYPE_CRIA_CLASS;
    klass->definition = definition;
    
    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



void*
CriaClass_get(
	Interpreter interpreter,
	CriaClass klass,
    String variableName
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    void* value = NULL;
    
    DefinitionClass definition = klass->definition;
    
    Hash fields = DefinitionClass_getFields(definition, TRUE);
    DefinitionVariable variable = (DefinitionVariable)Hash_get(fields, variableName);
    if (variable == NULL)
    {
    	Logger_err("Not found variable named '%s'", variableName);
        Runtime_error(interpreter, "Not found variable named '%s'", variableName);
    	goto END;
    }
    
    value = DefinitionVariable_getObject(variable);

END:
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



DefinitionVariable
CriaClass_getVariable(
    Interpreter interpreter,
    CriaClass klass,
    String variableName
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    
    DefinitionClass definition = klass->definition;
    
    Hash fields = DefinitionClass_getFields(definition, TRUE);
    DefinitionVariable variable = (DefinitionVariable)Hash_get(fields, variableName);
    if (variable == NULL)
    {
    	Logger_err("Not found variable named '%s'", variableName);
        Runtime_error(interpreter, "Not found variable named '%s'", variableName);
    	goto END;
    }

END:
    Logger_trc("[  END  ]%s", __func__);
    return variable;
}

