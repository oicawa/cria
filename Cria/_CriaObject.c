#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "CriaId.h"
#include "String.h"
#include "List.h"

#include "_CriaObject.h"



CriaId
CriaObject_new(
	String className,
    List fields
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject object = NULL;
    
    object = Memory_malloc(sizeof(struct CriaObjectTag));
    memset(object, 0x00, sizeof(struct CriaObjectTag));
    object->id.name = String_clone(className);
    object->id.type = CRIA_DATA_TYPE_CRIA_OBJECT;
    object->fields = fields;
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)object;
}


