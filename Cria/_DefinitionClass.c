#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "DefinitionFunction.h"
#include "Runtime.h"

#include "_DefinitionClass.h"



DefinitionClass
DefinitionClass_new(
	Interpreter interpreter,
    char*               name,
    Boolean             isNative,
    List                fieldList,
    List                methodList,
    CriaNativeClassLoader* loader
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionClass definition = NULL;
    
    if (isNative == TRUE)
    {
        Logger_dbg("Load Native Class");
        definition = (*loader)(interpreter, name);
        goto END;
    }
    
    Logger_dbg("Cria Function (ParameterCount=%d, StatementCount=%d)", List_count(fieldList), List_count(methodList));
    definition = Memory_malloc(sizeof(struct DefinitionClassTag));
    memset(definition, 0x00, sizeof(struct DefinitionClassTag));
    definition->fieldList = fieldList;
    definition->methodList = methodList;
    
    definition->name = String_new(name);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



DefinitionClass
DefinitionClass_search(
    List classList,
    char* name
)
{
    int count = List_count(classList);
    int index = 0;
    DefinitionClass definition = NULL;
    DefinitionClass tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionClass)(List_get(classList, index));
        if (strcmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
DefinitionClass_evaluate(
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
    function = DefinitionFunction_search(klass->methodList, name);
    if (function == NULL)
    {
    	Logger_err("Method is not found. (%s)/%d", name, List_count(klass->methodList));
        runtime_error(interpreter);
    }
    
    Logger_dbg("parameters->count = %d", List_count(parameters));
    value = DefinitionFunction_evaluate(interpreter, id, parameterList, function, parameters);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



List
DefinitionClass_getMethods(
	DefinitionClass klass
)
{
	return klass->methodList;
}
