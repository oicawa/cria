#include "_Cria.h"

FunctionDefinition
functionDefinition_new(
    char*               name,
    Boolean             isNative,
    AccessLevel         access,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
)
{
    FunctionDefinition definition = Memory_malloc(sizeof(struct FunctionDefinitionTag));
    memset(definition, 0x00, sizeof(struct FunctionDefinitionTag));
    definition->name = name;
    definition->isNative = isNative;
    definition->access = access;
    if (isNative == TRUE)
    {
        definition->of.native.function = nativeFunctionPoint;
    }
    else
    {
        definition->of.cria.parameterList = parameterList;
        definition->of.cria.statementList = statementList;
    }

    return definition;
}



FunctionDefinition
functionDefinition_search(
    List    functions,
    char*   name
)
{
    int count = functions->count;
    int index = 0;
    FunctionDefinition definition = NULL;
    FunctionDefinition tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (FunctionDefinition)(list_get(functions, index));
        if (strcmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}




