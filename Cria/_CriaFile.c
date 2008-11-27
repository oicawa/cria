#include "_Cria.h"



CriaId
CriaFile_new(
	Interpreter interpreter,
    List        args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId object = NULL;
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
    file->id.name = NULL;
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
    List        args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    CriaId object = NULL;
    CriaString path = NULL;
    CriaFile file = NULL;
    
    if (args->count != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    object = (CriaId)(list_get(args, 0));
    if (object->type != CRIA_DATA_TYPE_FILE)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaFile)object;
    
    file->pointer = fopen(path->value->pointer, "r");
    if (file->pointer == NULL)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaFile_close(
	Interpreter interpreter,
    List        args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    CriaId object = NULL;
    CriaFile file = NULL;
    
    if (args->count != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    object = (CriaId)(list_get(args, 0));
    if (object->type != CRIA_DATA_TYPE_FILE)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    file = (CriaFile)object;
    
    fclose(file->pointer);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaFile_read(
	Interpreter interpreter,
    List        args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = CriaId_new(NULL, CRIA_DATA_TYPE_VOID);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return id;
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



ClassDefinition
CriaFile_loadClass(
    Interpreter interpreter,
    char* className
)
{
    Logger_trc("[ START ]%s", __func__);
    String variableName = NULL;
    VariableDefinition variable = NULL;
    FunctionDefinition function = NULL;
    ClassDefinition klass = NULL;
    
    klass = Memory_malloc(sizeof(struct ClassDefinitionTag));
    memset(klass, 0x00, sizeof(struct ClassDefinitionTag));
    klass->fieldList = list_new();
    klass->methodList = list_new();
    klass->name = string_new(className);
    
    variableName = string_new("file");
    variable = variableDefinition_new(variableName);
    variable->isStatic = FALSE;
    string_dispose(variableName);
    list_add(klass->fieldList, variable);
    
    function = functionDefinition_new("new", TRUE, NULL, NULL, CriaFile_new);
    function->isStatic = TRUE;
    list_add(klass->fieldList, function);
    
    function = functionDefinition_new("open", TRUE, NULL, NULL, CriaFile_open);
    function->isStatic = FALSE;
    list_add(klass->fieldList, function);
    
    function = functionDefinition_new("close", TRUE, NULL, NULL, CriaFile_close);
    function->isStatic = FALSE;
    list_add(klass->fieldList, function);
    
    function = functionDefinition_new("read", TRUE, NULL, NULL, CriaFile_read);
    function->isStatic = FALSE;
    list_add(klass->fieldList, function);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



