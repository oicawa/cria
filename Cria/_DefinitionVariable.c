#include "_Cria.h"



DefinitionVariable
variableDefinition_new(
    String      name
)
{
    DefinitionVariable definition = Memory_malloc(sizeof(struct DefinitionVariableTag));
    memset(definition, 0x00, sizeof(struct DefinitionVariableTag));
    definition->name = string_clone(name);
    definition->isStatic = TRUE;

    return definition;
}



DefinitionVariable
variableDefinition_search(
    List    variables,
    String  name
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    DefinitionVariable tmp = NULL;
    int index = 0;
    Item item = NULL;
    
    Logger_dbg("variables pointer = %p", variables);
    if (variables == NULL)
    {
        Logger_dbg("Variables is NULL.");
        goto END;
    }
    
    if (name == NULL)
    {
        Logger_dbg("'name' is NULL.");
        goto END;
    }
    Logger_dbg("'name' is [%s].", name->pointer);
    
    Logger_dbg("Pre Loop start.");
    item = variables->item;
    Logger_dbg("Loop start.");
    for (index = 0; index < variables->count; index++)
    {
        Logger_dbg("Condition OK.");
        tmp = (DefinitionVariable)list_get(variables, index);
        Logger_dbg("tmp = %p", tmp);
        Logger_dbg("tmp->name = %p", tmp->name);
        Logger_dbg("tmp->name->pointer = %p", tmp->name->pointer);
        Logger_dbg("tmp->name->pointer = \"%s\"", tmp->name->pointer);
        if (strcmp(tmp->name->pointer, name->pointer) != 0)
            continue;
        
        definition = tmp;
        break;
    }
    Logger_dbg("Loop end.");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



