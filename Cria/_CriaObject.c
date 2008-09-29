#include "_Cria.h"


//*
CriaObject*
CriaObject_new(
    String          name,
    CriaDataType    type,
    void*           object
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject* criaObject;
    memset(criaObject, 0x00, sizeof(CriaObject));
    criaObject->id.name = string_clone(name);
    criaObject->id.type = type;
    criaObject->object = object;
    Logger_trc("[  END  ]%s", __func__);
    return criaObject;
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
