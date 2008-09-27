#include "_Cria.h"

FunctionDefinition
functionDefinition_new(
    char*       name,
    Boolean     isNative,
    AccessLevel access,
    List        parameterList,
    List        statementList,
    CriaObject  returnValue
)
{
    FunctionDefinition definition = Memory_malloc(sizeof(struct FunctionDefinitionTag));
    memset(definition, 0x00, sizeof(struct FunctionDefinitionTag));
    definition->name = name;
    definition->isNative = isNative;
    definition->access = access;
    if (isNative == TRUE)
        goto END;
    
    definition->of.cria.parameterList = parameterList;
    definition->of.cria.statementList = statementList;
    definition->returnValue = returnValue;
END:
    return definition;
}

