#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "CriaString.h"
#include "Runtime.h"
#include "StringBuffer.h"
#include "CriaBoolean.h"
#include "DefinitionVariable.h"
#include "DefinitionFunction.h"
#include "DefinitionClass.h"

#include "_CriaFile.h"



CriaId
CriaFile_new(
	Interpreter interpreter,
	CriaId object,
    List        args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString path = NULL;
    
    if (List_count(args) != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    object = (CriaId)(List_get(args, 0));
    if (object->type != CRIA_DATA_TYPE_STRING)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    path = (CriaString)object;
    
    
    CriaFile file = Memory_malloc(sizeof(struct CriaFileTag));
    memset(file, 0x00, sizeof(struct CriaFileTag));
    file->id.name = String_new("File");
    file->id.type = CRIA_DATA_TYPE_FILE;
    file->path = string_clone(path->value);
    file->pointer = NULL;
    
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
    CriaId id = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    CriaFile file = NULL;
    
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_FILE)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaFile)object;
    
    file->pointer = fopen(file->path, "r");
    if (file->pointer == NULL)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    Logger_inf("File opened. (%s)", file->path);
    
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
    CriaId id = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    CriaFile file = NULL;
    
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_FILE)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaFile)object;
    
    fclose(file->pointer);
    Logger_inf("File closed. (%s)", file->path);
    
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
    CriaFile file = NULL;
    
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_FILE)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaFile)object;
    stringBuffer = StringBuffer_new();
    
    while (feof(file->pointer) == 0)
    {
		memset(buffer, 0x00, 1024);
		fgets(buffer, 1024 - 1, file->pointer);
		length = strlen(buffer);
		if (length == 0)
			break;
		
		stringBuffer_append(stringBuffer, buffer);
		
		if (buffer[length - 1] == '\n')
			break;
    }
    
    String line = stringBuffer_toString(stringBuffer);
    stringBuffer_dispose(stringBuffer);
    
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
    CriaFile file = NULL;
    
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (object->type != CRIA_DATA_TYPE_FILE)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaFile)object;
    
    if (feof(file->pointer) != 0)
    {
    	result->value = TRUE;
    }
    Logger_inf("File EOF. (%s)", file->path);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)result;
}



void
CriaFile_dispose(
    CriaFile file
)
{
    Logger_trc("[ START ]%s", __func__);
    
    Memory_free(file);
    
    Logger_trc("[  END  ]%s", __func__);
}



DefinitionClass
CriaFile_loadClass(
    Interpreter interpreter,
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
    variable = definition_variable_new(variableName);
    Logger_dbg("4");
    string_dispose(variableName);
    Logger_dbg("5");
    list_add(fieldList, variable);
    Logger_dbg("6");
    
    function = definition_function_new("new", TRUE, FALSE, NULL, NULL, CriaFile_new);
    Logger_dbg("7");
    list_add(methodList, function);
    Logger_dbg("8");
    
    function = definition_function_new("open", TRUE, FALSE, NULL, NULL, CriaFile_open);
    Logger_dbg("9");
    list_add(methodList, function);
    Logger_dbg("10");
    
    function = definition_function_new("close", TRUE, FALSE, NULL, NULL, CriaFile_close);
    Logger_dbg("11");
    list_add(methodList, function);
    Logger_dbg("12");
    
    function = definition_function_new("read", TRUE, FALSE, NULL, NULL, CriaFile_read);
    Logger_dbg("13");
    list_add(methodList, function);
    Logger_dbg("14");
    
    
    function = definition_function_new("is_end", TRUE, FALSE, NULL, NULL, CriaFile_isEnd);
    Logger_dbg("15");
    list_add(methodList, function);
    Logger_dbg("16");
    
    
    klass = DefinitionClass_new(interpreter, className, FALSE, fieldList, methodList, NULL);
    Logger_dbg("17");

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



