#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "CriaString.h"
#include "Runtime.h"
#include "StringBuffer.h"
#include "CriaBoolean.h"
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
    List        args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaObject file = NULL;
    CriaString path = NULL;
    FILE* pointer = NULL;
    void* tmp = NULL;
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
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
    
    Logger_dbg("Path = '%s'", path->value);
    pointer = fopen(path->value, "r");
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
    
    
    stringBuffer = StringBuffer_new();
    
    while (feof(pointer) == 0)
    {
		memset(buffer, 0x00, 1024);
		fgets(buffer, 1024 - 1, pointer);
		length = strlen(buffer);
		if (length == 0)
			break;
		
		StringBuffer_append(stringBuffer, buffer);
		
		if (buffer[length - 1] == '\n')
			break;
    }
    
    String line = StringBuffer_toString(stringBuffer);
    StringBuffer_dispose(stringBuffer);
    
    CriaString string = CriaString_new(TRUE, line);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)string;
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
    String variableName = NULL;
    DefinitionVariable variable = NULL;
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    List fieldList = List_new();
    Logger_dbg("1");
    List methodList = List_new();
    Logger_dbg("2");
    
    variableName = String_new("file");
    Logger_dbg("3");
    variable = DefinitionVariable_new(variableName, FALSE);
    Logger_dbg("4");
    String_dispose(variableName);
    Logger_dbg("5");
    List_add(fieldList, variable);
    Logger_dbg("6");
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaFile__generator_);
    Logger_dbg("7");
    List_add(methodList, function);
    Logger_dbg("8");
    
    function = DefinitionFunction_new("new", TRUE, FALSE, NULL, NULL, CriaFile_new);
    Logger_dbg("7");
    List_add(methodList, function);
    Logger_dbg("8");
    
    function = DefinitionFunction_new("open", TRUE, FALSE, NULL, NULL, CriaFile_open);
    Logger_dbg("9");
    List_add(methodList, function);
    Logger_dbg("10");
    
    function = DefinitionFunction_new("close", TRUE, FALSE, NULL, NULL, CriaFile_close);
    Logger_dbg("11");
    List_add(methodList, function);
    Logger_dbg("12");
    
    function = DefinitionFunction_new("read", TRUE, FALSE, NULL, NULL, CriaFile_read);
    Logger_dbg("13");
    List_add(methodList, function);
    Logger_dbg("14");
    
    
    function = DefinitionFunction_new("is_end", TRUE, FALSE, NULL, NULL, CriaFile_isEnd);
    Logger_dbg("15");
    List_add(methodList, function);
    Logger_dbg("16");
    
    
    klass = DefinitionClass_new(className, TRUE, fieldList, methodList, NULL);
    Logger_dbg("17");

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



