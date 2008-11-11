#include "_Cria.h"



VariableDefinition
variableDefinition_new(
    String      name
)
{
    VariableDefinition definition = Memory_malloc(sizeof(struct VariableDefinitionTag));
    memset(definition, 0x00, sizeof(struct VariableDefinitionTag));
    definition->name = string_clone(name);

    return definition;
}



VariableDefinition
variableDefinition_search(
    List    variables,
    String  name
)
{
    int count = variables->count;
    int index = 0;
    VariableDefinition definition = NULL;
    VariableDefinition tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (VariableDefinition)(list_get(variables, index));
        if (strcmp(tmp->name->pointer, name->pointer) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



