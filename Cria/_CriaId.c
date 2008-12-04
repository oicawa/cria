#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_CriaId.h"

//*
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
//*/



/*
void
CriaObject_dispose(
    CriaObject* criaObject
)
{
    Logger_cor("[ START ]%s", __func__);
    if (criaObject == NULL)
    {
        Logger_cor("Cria object is NULL.");
        goto END;
    }
    
    Logger_cor("Check criaObject->name.");
    if (criaObject->name != NULL)
    {
        Logger_cor("Free criaObject->name.");
        string_dispose(criaObject->name);
        Logger_cor("Set NULL to criaObject->name.");
        criaObject->name = NULL;
    }
    
    Logger_cor("Free criaObject.");
    Memory_free(criaObject);
    Logger_cor("Set NULL to criaObject.");
    criaObject = NULL;
    
END:
    Logger_cor("[  END  ]%s", __func__);
}
//*/
