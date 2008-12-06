#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_CriaId.h"


CriaId
CriaId_new(
    String          name,
    CriaDataType    type
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = Memory_malloc(sizeof(struct CriaIdTag));
    memset(id, 0x00, sizeof(struct CriaIdTag));
    id->name = string_clone(name);
    id->type = type;
    Logger_trc("[  END  ]%s", __func__);
    return id;
}


