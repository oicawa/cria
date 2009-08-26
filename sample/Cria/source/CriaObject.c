#include "Cria.h"
#include "Memory.h"
#include "Logger.h"

#include "CriaId.h"
#include "String.h"
#include "List.h"
#include "Hash.h"
#include "Definition.h"
#include "Runtime.h"

#include "CriaObject.h"



CriaObject
CriaObject_new(
	String className
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    CriaObject object = NULL;
    
    object = Memory_malloc(sizeof(struct CriaObjectTag));
    object->id.name = String_new(className);
    object->id.type = CRIA_DATA_TYPE_CRIA_OBJECT;
    object->fields = Hash_new(16);
    
    Logger_trc("[  END  ]%s", __func__);
    return object;
}



CriaObject
CriaObject_getObject(
	CriaId id,
    String class_name
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject object = NULL;

    
    Logger_dbg("Check object data type.");
    if (id->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	goto END;
    }
    
    if (strcmp(id->name, class_name) != 0)
    {
    	goto END;
    }
    
    object = (CriaObject)id;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return object;
}



void
CriaObject_addField(
	CriaObject object,
    DefinitionVariable variable
)
{
    Logger_trc("[ START ]%s", __func__);
    
    Hash_put(object->fields, DefinitionVariable_name(variable), variable);
    
    Logger_trc("[  END  ]%s", __func__);
    return;
}



void*
CriaObject_get(
	Interpreter interpreter,
	CriaObject object,
    String name
)
{
    Logger_trc("[ START ]%s", __func__);
    void* value = NULL;
    DefinitionVariable variable = NULL;
    
    
    //variable = DefinitionVariable_search(object->fields, name);
    variable = (DefinitionVariable)Hash_get(object->fields, name);
    if (variable == NULL)
    {
    	Logger_err("Not found variable named '%s'", name);
    	goto END;
    }
    
    
    value = DefinitionVariable_getObject(variable);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



void
CriaObject_set(
	Interpreter interpreter,
	CriaObject object,
    String name,
    void* value
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable variable = NULL;
    
    Logger_dbg("variable name = '%s'", name);
    variable = (DefinitionVariable)Hash_get(object->fields, name);
    Logger_dbg("fields = '%p'", object->fields);
    if (variable == NULL)
    {
    	Runtime_error(interpreter, "Variable is NULL.");
    	goto END;
    }
    
    DefinitionVariable_set(variable, value);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}




