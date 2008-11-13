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
    Logger_trc("[ START ]%s", __func__);
    VariableDefinition definition = NULL;
    VariableDefinition tmp = NULL;
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
    
    Logger_dbg("Pre Loop start.");
    item = variables->item;
    Logger_dbg("Loop start.");
    while (1)
    {
        if (item == NULL)
        {
            Logger_dbg("item is null.");
            break;
        }
        
        tmp = (VariableDefinition)(item->object);
        if (tmp == NULL)
        {
            Logger_dbg("object is null.");
            item = item->next;
            continue;
        }
        
        if (tmp->name == NULL)
        {
            Logger_dbg("object->name or name is null.");
            item = item->next;
            continue;
        }
        
        if (strcmp(tmp->name->pointer, name->pointer) == 0)
        {
            definition = tmp;
            break;
        }
    }
    Logger_dbg("Loop end.");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



