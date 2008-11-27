#include <stdio.h>


#include "_Cria.h"


ClassDefinition
classDefinition_new(
    char*               name,
    Boolean             isNative,
    List                fieldList,
    List                methodList,
    CriaNativeFunction* nativeFunctionPoint
)
{
    ClassDefinition definition = NULL;
    
    if (isNative == TRUE)
    {
        Logger_dbg("Load Native Class");
        definition = (*nativeFunctionPoint)(interpreter, name);
        goto END;
    }
    
    Logger_dbg("Cria Function (ParameterCount=%d, StatementCount=%d)", fieldList->count, methodList->count);
    definition = Memory_malloc(sizeof(struct ClassDefinitionTag));
    memset(definition, 0x00, sizeof(struct ClassDefinitionTag));
    definition->fieldList = fieldList;
    definition->methodList = methodList;
    
    definition->name = name;
    
END:
    return definition;
}



ClassDefinition
classDefinition_search(
    List classList,
    char* name
)
{
    int count = classList->count;
    int index = 0;
    ClassDefinition definition = NULL;
    ClassDefinition tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (ClassDefinition)(list_get(classList, index));
        if (strcmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
classDefinition_evaluate(
    Interpreter interpreter,
    CriaId  id,
    List parameterList,
    char*   name,
    ClassDefinition klass,
    List parameters
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId value = NULL;
    FunctionDefinition function = NULL;
    
    function = functionDefinition_search(klass->methodList, name);
    if (function == NULL)
        runtime_error(interpreter);
    
    
    value = functionDefinition_evaluate(interpreter, id, parameterList, function, parameters);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



