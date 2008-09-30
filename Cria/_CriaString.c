#include "_Cria.h"


//*
CriaString
CriaString_new(
    String          name,
    Boolean         isLiteral,
    String          value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString criaString = Memory_malloc(sizeof(struct CriaStringTag));
    memset(criaString, 0x00, sizeof(struct CriaStringTag));
    
    criaString->id.name = string_clone(name);
    criaString->id.type = CRIA_DATA_TYPE_STRING;
    criaString->isLiteral = isLiteral;
    criaString->value = string_clone(value);
    
    Logger_trc("[  END  ]%s", __func__);
    return criaString;
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
