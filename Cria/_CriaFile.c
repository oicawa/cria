#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "CriaString.h"
#include "Runtime.h"
#include "Hash.h"
#include "StringBuffer.h"
#include "CriaBoolean.h"
#include "CriaInteger.h"
#include "CriaObject.h"
#include "Definition.h"

#include "_CriaFile.h"



CriaId
CriaFile__generator_(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject file = CriaObject_new("File");
    CriaObject_addField(file, DefinitionVariable_new("path", FALSE));
    CriaObject_addField(file, DefinitionVariable_new("pointer", FALSE));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)file;
}



CriaId
CriaFile_new(
	Interpreter interpreter,
	CriaId object,
    List        args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString path = NULL;
    CriaObject file = NULL;
    CriaId arg = NULL;
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    if (List_count(args) != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    arg = (CriaId)(List_get(args, 0));
    if (arg->type != CRIA_DATA_TYPE_STRING)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    path = (CriaString)arg;
    
    
    file = (CriaObject)object;
    
    CriaObject_set(interpreter, file, "path", CriaString_new(TRUE, path->value));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)file;
}



CriaId
CriaFile_open(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaObject file = NULL;
    CriaString path = NULL;
    FILE* pointer = NULL;
    void* tmp = NULL;
    CriaId arg = NULL;
    String mode = NULL;

    
    Logger_dbg("Check object data type.");
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    Logger_dbg("Cast object from CriaId.");
    file = (CriaObject)object;
    Logger_dbg("Casted object from CriaId.");
    tmp = CriaObject_get(interpreter, file, "path");
    Logger_dbg("object = %p.", tmp);
    path = (CriaString)tmp;
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    arg = (CriaId)(List_get(args, 0));
    if (arg->type != CRIA_DATA_TYPE_STRING)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    mode = ((CriaString)arg)->value;
    
    
    Logger_dbg("Path = '%s'", path->value);
    Logger_dbg("Mode = '%s'", mode);
    pointer = fopen(path->value, mode);
    if (pointer == NULL)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    Logger_inf("File opened. (%s)", path->value);
    
    CriaObject_set(interpreter, file, "pointer", pointer);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaFile_close(
	Interpreter interpreter,
	CriaId object,
    List        args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaObject file = NULL;
    FILE* pointer = NULL;
    
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaObject)object;
    
    pointer = (FILE*)CriaObject_get(interpreter, file, "pointer");
    
    fclose(pointer);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaFile_read(
	Interpreter interpreter,
	CriaId object,
    List        args
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
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_dbg("object->type != CRIA_DATA_TYPE_CRIA_OBJECT");
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    file = (CriaObject)object;
    pointer = (FILE*)CriaObject_get(interpreter, file, "pointer");
    Logger_dbg("pointer is %p", pointer);
    
    
    stringBuffer = StringBuffer_new();
    
    Logger_dbg("Loop start.");
    while (feof(pointer) == 0)
    {
		memset(buffer, 0x00, 1024);
		fgets(buffer, 1024 - 1, pointer);
	    Logger_dbg("buffer = '%s'", buffer);
		length = strlen(buffer);
		if (length == 0)
			break;
		
		StringBuffer_append(stringBuffer, buffer);
		
		if (buffer[length - 1] == '\n')
			break;
    }
    Logger_dbg("Loop end.");
    
    String line = StringBuffer_toString(stringBuffer);
    StringBuffer_dispose(stringBuffer);
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
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject file = NULL;
    FILE* pointer = NULL;
    
    
    CriaId id;
    char* start = NULL;
    char* end = NULL;
    long index = 0;
    int count = 0;
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    file = (CriaObject)object;
    pointer = (FILE*)CriaObject_get(interpreter, file, "pointer");
    
    
    count = List_count(args);
    if (count == 0)
    {
        runtime_error(interpreter);
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
        printf("%s", string->value);
        CriaString_dispose(string);
        goto END;
    }
    
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        runtime_error(interpreter);
    }
    
    
	Logger_dbg("String");
	start = ((CriaString)id)->value;
	while((end = strstr(start, "%s")) != NULL)
	{
		long size = end - start;
		char* buffer = Memory_malloc(size + 1);
		memset(buffer, 0x00, size + 1);
		strncpy(buffer, start, size);
		fprintf(pointer, buffer);
		Memory_free(buffer);
		buffer = NULL;
		start = end + 2;
		
		if (count < index)
		{
			runtime_error(interpreter);
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
			fprintf(pointer, "%s", ((CriaString)id)->value);
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
			fprintf(pointer, "%s", string->value);
			CriaString_dispose(string);
			continue;
		}
		
		Logger_err("id->type = %d", id->type);
		runtime_error(interpreter);
	}
	Logger_dbg("[print]%s", start);
	fprintf(pointer, start);
    
END:
    fflush(pointer);
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



CriaId
CriaFile_isEnd(
	Interpreter interpreter,
	CriaId object,
    List        args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaBoolean result = CriaBoolean_new(TRUE, FALSE);
    CriaObject file = NULL;
    FILE* pointer = NULL;
    
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaObject)object;
    pointer = (FILE*)CriaObject_get(interpreter, file, "pointer");
    
    if (feof(pointer) != 0)
    {
    	result->value = TRUE;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)result;
}



void
CriaFile_dispose(
    CriaObject file
)
{
    Logger_trc("[ START ]%s", __func__);
    
    Memory_free(file);
    
    Logger_trc("[  END  ]%s", __func__);
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
    
    variable = DefinitionVariable_new("file", FALSE);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaFile__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaFile_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("open", TRUE, FALSE, NULL, NULL, CriaFile_open);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("close", TRUE, FALSE, NULL, NULL, CriaFile_close);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("read", TRUE, FALSE, NULL, NULL, CriaFile_read);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("write", TRUE, FALSE, NULL, NULL, CriaFile_write);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("is_end", TRUE, FALSE, NULL, NULL, CriaFile_isEnd);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);
    Logger_dbg("17");

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



