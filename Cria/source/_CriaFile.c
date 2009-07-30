#include <string.h>

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
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject file = CriaObject_new("File");
    CriaObject_addField(file, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "path", FALSE, FALSE, NULL));
    CriaObject_addField(file, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "pointer", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)file;
}



CriaId
CriaFile_new(
	Interpreter interpreter,
	CriaId object,
    List args
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
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
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
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    
    arg = (CriaId)(List_get(args, 0));
    if (arg->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "1st argument is not string.");
    	goto END;
    }
    mode = ((CriaString)arg)->value;
    
    
    Logger_dbg("Path = '%s'", path->value);
    Logger_dbg("Mode = '%s'", mode);
    pointer = fopen(path->value, mode);
    if (pointer == NULL)
    {
    	Runtime_error(interpreter, "File open error. (%s)", path->value);
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
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
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
    pointer = (FILE*)CriaObject_get(interpreter, file, "pointer");
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
		
		StringBuffer_append(stringBuffer, buffer);
		
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
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    file = (CriaObject)object;
    pointer = (FILE*)CriaObject_get(interpreter, file, "pointer");
    
    
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
        printf("%d", value);
        goto END;
    }
    
    if (id->type == CRIA_DATA_TYPE_BOOLEAN)
    {
        Logger_dbg("Boolean");
        CriaString string = (CriaString)CriaBoolean_toString(interpreter, (CriaBoolean)id);
        printf("%s", string->value);
        goto END;
    }
    
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        Runtime_error(interpreter, "Data type of 1st argument is not Integer, Boolean, and String.")    ;
    }
    
    
	Logger_dbg("String");
	start = ((CriaString)id)->value;
	while((end = strstr(start, "%s")) != NULL)
	{
		long size = end - start;
		char* buffer = Memory_malloc(size + 1);
		strncpy(buffer, start, size);
		fprintf(pointer, buffer);
		buffer = NULL;
		start = end + 2;
		
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
			continue;
		}
		
		Logger_err("id->type = %d", id->type);
        Runtime_error(interpreter, "Data type of argument %d is not Integer, Boolean, and String.", index + 1);
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
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
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
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "file", FALSE, FALSE, NULL);
    
    Logger_dbg("0");
    
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    Logger_dbg("1");
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaFile__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("2");
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaFile_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("3");
    
    function = DefinitionFunction_new("open", TRUE, FALSE, NULL, NULL, CriaFile_open);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("4");
    
    function = DefinitionFunction_new("close", TRUE, FALSE, NULL, NULL, CriaFile_close);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("5");
    
    function = DefinitionFunction_new("read", TRUE, FALSE, NULL, NULL, CriaFile_read);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("6");
    
    function = DefinitionFunction_new("write", TRUE, FALSE, NULL, NULL, CriaFile_write);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("7");
    
    function = DefinitionFunction_new("is_end", TRUE, FALSE, NULL, NULL, CriaFile_isEnd);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    Logger_dbg("Create class definition.");
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);
    Logger_dbg("17");

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



