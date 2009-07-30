#include "Memory.h"
#include "Logger.h"

#include "Cria.h"
#include "CriaId.h"


CriaId
CriaId_new(
    String          name,
    CriaDataType    type
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = Memory_malloc(sizeof(struct CriaIdTag));
    id->name = String_clone(name);
    id->type = type;
    Logger_trc("[  END  ]%s", __func__);
    return id;
}


