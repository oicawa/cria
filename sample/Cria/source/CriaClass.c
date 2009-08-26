#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "CriaId.h"
#include "String.h"
#include "Hash.h"
#include "Definition.h"
#include "Runtime.h"
#include "CriaClass.h"



CriaClass
CriaClass_new(
	DefinitionClass definition
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    CriaClass klass = NULL;
    
    klass = Memory_malloc(sizeof(struct CriaClassTag));
    klass->id.name = String_new("Class");
    klass->id.type = CRIA_DATA_TYPE_CRIA_CLASS;
    klass->definition = definition;
    
    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



void*
CriaClass_get(
	Interpreter interpreter,
	CriaClass klass,
    String variableName
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    void* value = NULL;
    
    DefinitionClass definition = klass->definition;
    
    Hash fields = DefinitionClass_getFields(definition, TRUE);
    DefinitionVariable variable = (DefinitionVariable)Hash_get(fields, variableName);
    if (variable == NULL)
    {
    	Logger_err("Not found variable named '%s'", variableName);
        Runtime_error(interpreter, "Not found variable named '%s'", variableName);
    	goto END;
    }
    
    value = DefinitionVariable_getObject(variable);

END:
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



DefinitionVariable
CriaClass_getVariable(
    Interpreter interpreter,
    CriaClass klass,
    String variableName
)
{
    Logger_trc("[ START ]%s(class name = '%s')", __func__);
    
    DefinitionClass definition = klass->definition;
    
    Hash fields = DefinitionClass_getFields(definition, TRUE);
    DefinitionVariable variable = (DefinitionVariable)Hash_get(fields, variableName);
    if (variable == NULL)
    {
    	Logger_err("Not found variable named '%s'", variableName);
        Runtime_error(interpreter, "Not found variable named '%s'", variableName);
    	goto END;
    }

END:
    Logger_trc("[  END  ]%s", __func__);
    return variable;
}
