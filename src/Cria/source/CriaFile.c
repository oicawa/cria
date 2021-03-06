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
#include "CriaString.h"
#include "Runtime.h"
#include "Hash.h"
#include "StringBuffer.h"
#include "CriaBoolean.h"
#include "CriaInteger.h"
#include "CriaObject.h"
#include "Definition.h"
#include "CriaFile.h"


CriaId
CriaFile__generator_(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject file = CriaObject_new(L"File");
    CriaObject_addField(file, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, L"path", FALSE, FALSE, NULL));
    CriaObject_addField(file, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, L"pointer", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)file;
}



CriaId
CriaFile_new(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString path = NULL;
    CriaObject file = NULL;
    CriaId arg = NULL;
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	Runtime_error(interpreter, "Data type of target object is illegal.");
    	goto END;
    }
    
    
    if (List_count(args) != 1)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    arg = (CriaId)(List_get(args, 0));
    if (arg->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "1st argument is not String.");
    	goto END;
    }
    
    path = (CriaString)arg;
    
    
    file = (CriaObject)object;
    
    CriaObject_set(interpreter, file, L"path", CriaString_new(TRUE, path->value));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)file;
}



CriaId
CriaFile_open(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaObject file = NULL;
    FILE* pointer = NULL;
    CriaId arg = NULL;
    String path = NULL;
    String mode = NULL;

    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 2)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    arg = (CriaId)List_get(args, 0);
    if (arg->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    path = ((CriaString)arg)->value;
    
    
    arg = (CriaId)(List_get(args, 1));
    if (arg->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "1st argument is not string.");
    	goto END;
    }
    mode = ((CriaString)arg)->value;
    
    
    file = (CriaObject)CriaFile__generator_(interpreter, object, args, NULL);
    
    
    pointer = fopen(String_wcsrtombs(path), String_wcsrtombs(mode));
    if (pointer == NULL)
    {
    	Runtime_error(interpreter, "File open error. (%s)", path);
    	goto END;
    }
    
    CriaObject_set(interpreter, file, L"pointer", pointer);
    
    
    
    //TODO: Implement 'block' logic.
    if (block == NULL)
    {
        id = (CriaId)file;
        goto END;
    }
    
    List parameters = List_new();
    List_add(parameters, file);
    CriaBlock_evaluate(block, parameters);
    
    
    fclose(pointer);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaFile_close(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaObject file = NULL;
    FILE* pointer = NULL;
    
    if (List_count(args) != 0)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    
    file = (CriaObject)object;
    
    pointer = (FILE*)CriaObject_get(interpreter, file, L"pointer");
    
    fclose(pointer);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaFile_read(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    char buffer[1024];
    StringBuffer stringBuffer = NULL;
    int length = 0;
    CriaObject file = NULL;
    FILE* pointer = NULL;
    
    if (List_count(args) != 0)
    {
    	Logger_dbg("args count != 0");
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_dbg("object->type != CRIA_DATA_TYPE_CRIA_OBJECT");
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    
    
    file = (CriaObject)object;
    pointer = (FILE*)CriaObject_get(interpreter, file, L"pointer");
    Logger_dbg("pointer is %p", pointer);
    
    
    stringBuffer = StringBuffer_new();
    
    Logger_dbg("Loop start.");
    while (feof(pointer) == 0)
    {
		Memory_reset(buffer, 1024);
		fgets(buffer, 1024 - 1, pointer);
	    Logger_dbg("buffer = '%s'", buffer);
		length = strlen(buffer);
		if (length == 0)
			break;
		
		StringBuffer_append(stringBuffer, String_mbsrtowcs(buffer));
		
		if (buffer[length - 1] == '\n')
			break;
    }
    Logger_dbg("Loop end.");
    
    String line = StringBuffer_toString(stringBuffer);
    Logger_dbg("line = '%s'", line);
    
    CriaString string = CriaString_new(TRUE, line);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)string;
}



CriaId
CriaFile_write(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject file = NULL;
    FILE* pointer = NULL;
    
    
    CriaId id;
    String start = NULL;
    String end = NULL;
    long index = 0;
    int count = 0;
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    file = (CriaObject)object;
    pointer = (FILE*)CriaObject_get(interpreter, file, L"pointer");
    
    
    count = List_count(args);
    if (count == 0)
    {
        Runtime_error(interpreter, "Illegal arguments count.");
    }
    
    id = (CriaId)List_get(args, 0);
    
    
    if (id->type == CRIA_DATA_TYPE_INTEGER)
    {
        Logger_dbg("Integer");
        int value = ((CriaInteger)id)->value;
        fprintf(pointer, "%d", value);
        goto END;
    }
    
    if (id->type == CRIA_DATA_TYPE_BOOLEAN)
    {
        Logger_dbg("Boolean");
        CriaString string = (CriaString)CriaBoolean_toString(interpreter, (CriaBoolean)id);
        fprintf(pointer, "%s", String_wcsrtombs(string->value));
        goto END;
    }
    
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        Runtime_error(interpreter, "Data type of 1st argument is not Integer, Boolean, and String.")    ;
    }
    
    
	Logger_dbg("String");
	start = ((CriaString)id)->value;
	while((end = wcsstr(start, L"%s")) != NULL)
	{
		long size = wcslen(start) - wcslen(end);
		String buffer = Memory_malloc(sizeof(wchar_t) * (size + 1));
		wcsncpy(buffer, start, size);
		fprintf(pointer, String_wcsrtombs(buffer));
		buffer = NULL;
		start = &end[2];
		
		if (count < index)
		{
			Runtime_error(interpreter, "Argument index is over parameters count.");
		}
		
		index += 1;
		id = (CriaId)List_get(args, index);
		
		if (id == NULL)
		{
			fprintf(pointer, "(null)");
			continue;
		}
		
		if (id->type == CRIA_DATA_TYPE_STRING)
		{
			Logger_dbg("[print]%s", ((CriaString)id));
			fprintf(pointer, "%s", String_wcsrtombs(((CriaString)id)->value));
			continue;
		}
		
		if (id->type == CRIA_DATA_TYPE_INTEGER)
		{
			fprintf(pointer, "%d", ((CriaInteger)id)->value);
			continue;
		}
		
		if (id->type == CRIA_DATA_TYPE_BOOLEAN)
		{
			CriaString string = (CriaString)CriaBoolean_toString(interpreter, (CriaBoolean)id);
			fprintf(pointer, "%s", String_wcsrtombs(string->value));
			continue;
		}
		
		Logger_err("id->type = %d", id->type);
        Runtime_error(interpreter, "Data type of argument %d is not Integer, Boolean, and String.", index + 1);
	}
	fprintf(pointer, String_wcsrtombs(start));
    
END:
    fflush(pointer);
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



CriaId
CriaFile_isEnd(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaBoolean result = CriaBoolean_new(TRUE, FALSE);
    CriaObject file = NULL;
    FILE* pointer = NULL;
    
    if (List_count(args) != 0)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    
    file = (CriaObject)object;
    pointer = (FILE*)CriaObject_get(interpreter, file, L"pointer");
    
    if (feof(pointer) != 0)
    {
    	result->value = TRUE;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)result;
}



DefinitionClass
CriaFile_loadClass(
    String className
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable variable = NULL;
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    Hash i_fields = Hash_new(32);
    Hash s_fields = Hash_new(32);
    Hash i_methods = Hash_new(32);
    Hash s_methods = Hash_new(32);
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, L"file", FALSE, FALSE, NULL);
    
    Logger_dbg("0");
    
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    Logger_dbg("1");
    
    function = DefinitionFunction_new(L" generator ", TRUE, TRUE, NULL, NULL, CriaFile__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("2");
    
    function = DefinitionFunction_new(L"new", TRUE, TRUE, NULL, NULL, CriaFile_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("3");
    
    function = DefinitionFunction_new(L"open", TRUE, TRUE, NULL, NULL, CriaFile_open);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("4");
    
    function = DefinitionFunction_new(L"close", TRUE, FALSE, NULL, NULL, CriaFile_close);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("5");
    
    function = DefinitionFunction_new(L"read", TRUE, FALSE, NULL, NULL, CriaFile_read);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("6");
    
    function = DefinitionFunction_new(L"write", TRUE, FALSE, NULL, NULL, CriaFile_write);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("7");
    
    function = DefinitionFunction_new(L"is_end", TRUE, FALSE, NULL, NULL, CriaFile_isEnd);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("Create class definition.");
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);
    Logger_dbg("17");

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



