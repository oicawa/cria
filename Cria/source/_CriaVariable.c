#include <string.h>

#include "Memory.h"
#include "Logger.h"

#include "CriaBoolean.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"
#include "CriaObject.h"
#include "CriaInteger.h"
#include "Definition.h"

#include "_CriaVariable.h"



CriaId
CriaVariable_new(
    String name,
	DefinitionVariable definition
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaVariable variable = NULL;
    
    variable = Memory_malloc(sizeof(struct CriaVariableTag));
    variable->id.name = String_clone(name);
    variable->id.type = CRIA_DATA_TYPE_VARIABLE;
    variable->definition = definition;
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)variable;
}


