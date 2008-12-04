#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_DefinitionFunction.h"
#include "_Runtime.h"

#include "_DefinitionClass.h"


DefinitionClass
definition_class_new(
	Interpreter interpreter,
    char*               name,
    Boolean             isNative,
    List                fieldList,
    List                methodList,
    CriaNativeClassLoader* loader
)
{
    DefinitionClass definition = NULL;
    
    if (isNative == TRUE)
    {
        Logger_dbg("Load Native Class");
        definition = (*loader)(interpreter, name);
        goto END;
    }
    
    Logger_dbg("Cria Function (ParameterCount=%d, StatementCount=%d)", fieldList->count, methodList->count);
    definition = Memory_malloc(sizeof(struct DefinitionClassTag));
    memset(definition, 0x00, sizeof(struct DefinitionClassTag));
    definition->fieldList = fieldList;
    definition->methodList = methodList;
    
    definition->name = string_new(name);
    
END:
    return definition;
}



DefinitionClass
definition_class_search(
    List classList,
    char* name
)
{
    int count = classList->count;
    int index = 0;
    DefinitionClass definition = NULL;
    DefinitionClass tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionClass)(list_get(classList, index));
        if (strcmp(tmp->name->pointer, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
definition_class_evaluate(
    Interpreter interpreter,
    CriaId  id,
    List parameterList,
    char*   name,
    DefinitionClass klass,
    List parameters
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId value = NULL;
    DefinitionFunction function = NULL;
    
    Logger_dbg("Method name is '%s'", name);
    Logger_dbg("klass is '%p'", klass);
    Logger_dbg("klass->methodList is '%p'", klass->methodList);
    function = definition_function_search(klass->methodList, name);
    if (function == NULL)
    {
    	Logger_err("Method is not found. (%s)/%d", name, klass->methodList->count);
        runtime_error(interpreter);
    }
    
    Logger_dbg("parameters->count = %d", parameters->count);
    value = definition_function_evaluate(interpreter, id, parameterList, function, parameters);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



