#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "String.h"
#include "List.h"

#include "_DefinitionVariable.h"



DefinitionVariable
definition_variable_new(
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
definition_variable_search(
    List    variables,
    String  name
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    DefinitionVariable tmp = NULL;
    int index = 0;
    
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
    Logger_dbg("'name' is [%s].", name);
    
    Logger_dbg("Loop start.");
    for (index = 0; index < list_count(variables); index++)
    {
        Logger_dbg("Condition OK.");
        tmp = (DefinitionVariable)list_get(variables, index);
        Logger_dbg("tmp = %p", tmp);
        Logger_dbg("tmp->name = %s", tmp->name);
        if (strcmp(tmp->name, name) != 0)
            continue;
        
        definition = tmp;
        break;
    }
    Logger_dbg("Loop end.");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



String
definition_variable_name(
	DefinitionVariable variable
)
{
	return variable->name;
}



void
definition_variable_set(
	DefinitionVariable variable,
	CriaId id
)
{
	variable->object = id;
}



CriaId
DefinitionVariable_getObject(
	DefinitionVariable variable
)
{
	return variable->object;
}