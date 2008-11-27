#include "_Cria.h"



ClassDefinition
CriaFile_loadClass(
    Interpreter interpreter,
    char* name
)
{
    Logger_trc("[ START ]%s", __func__);
    ClassDefinition definition = Memory_malloc(sizeof(struct ClassDefinitionTag));
    memset(definition, 0x00, sizeof(struct ClassDefinitionTag));
    definition->fieldList = list_new();
    definition->methodList = list_new();
    definition->name = name;
    
    list_add(definition->filedList, "file");
    
    list_add(definition->filedList, "file");
    
    Logger_trc("[  END  ]%s", __func__);
    return criaString;
}



CriaFile
CriaFile_new(
    String          path
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



void
CriaFile_dispose(
    CriaFile criaFile
)
{
    Logger_trc("[ START ]%s", __func__);
    
    string_dispose(criaString->id.name);
    string_dispose(criaString->value);
    
    Memory_free(criaString);
    
    Logger_trc("[  END  ]%s", __func__);
}



