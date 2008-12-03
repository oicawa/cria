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
    
    if (args->count != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    object = (CriaId)(list_get(args, 0));
    if (object->type != CRIA_DATA_TYPE_STRING)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    path = (CriaString)object;
    
    
    CriaFile file = Memory_malloc(sizeof(struct CriaFileTag));
    memset(file, 0x00, sizeof(struct CriaFileTag));
    file->id.name = string_new("File");
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
    
    if (args->count != 0)
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
    
    file->pointer = fopen(file->path->pointer, "r");
    if (file->pointer == NULL)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    Logger_inf("File opened. (%s)", file->path->pointer);
    
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
    
    if (args->count != 0)
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
    Logger_inf("File closed. (%s)", file->path->pointer);
    
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
    
    if (args->count != 0)
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
    stringBuffer = stringBuffer_new();
    
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
    
    if (args->count != 0)
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
    Logger_inf("File EOF. (%s)", file->path->pointer);
    
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
    char* className
)
{
    Logger_trc("[ START ]%s", __func__);
    String variableName = NULL;
    DefinitionVariable variable = NULL;
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;
    
    klass = Memory_malloc(sizeof(struct DefinitionClassTag));
    memset(klass, 0x00, sizeof(struct DefinitionClassTag));
    klass->fieldList = list_new();
    klass->methodList = list_new();
    klass->name = string_new(className);
    
    variableName = string_new("file");
    variable = definition_variable_new(variableName);
    variable->isStatic = FALSE;
    string_dispose(variableName);
    list_add(klass->fieldList, variable);
    
    function = definition_function_new("new", TRUE, NULL, NULL, CriaFile_new);
    function->isStatic = TRUE;
    list_add(klass->methodList, function);
    
    function = definition_function_new("open", TRUE, NULL, NULL, CriaFile_open);
    function->isStatic = FALSE;
    list_add(klass->methodList, function);
    
    function = definition_function_new("close", TRUE, NULL, NULL, CriaFile_close);
    function->isStatic = FALSE;
    list_add(klass->methodList, function);
    
    function = definition_function_new("read", TRUE, NULL, NULL, CriaFile_read);
    function->isStatic = FALSE;
    list_add(klass->methodList, function);
    
    
    function = definition_function_new("is_end", TRUE, NULL, NULL, CriaFile_isEnd);
    function->isStatic = FALSE;
    list_add(klass->methodList, function);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



