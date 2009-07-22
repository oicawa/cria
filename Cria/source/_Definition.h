#ifndef PRIVATE_DEFINITION_H_INCLUDED
#define PRIVATE_DEFINITION_H_INCLUDED

#include "Cria.h"
#include "Definition.h"



struct DefinitionVariableNormalTag
{
    void* object;
    String name;
    Boolean isStatic;
    Boolean isConstant;
};


struct DefinitionVariableTag
{
    DefinitionVariableType type;
    union
    {
        DefinitionVariableNormal normal;
        Item item;
    } of;
};



struct DefinitionFunctionTag
{
    char* name;
    Boolean isNative;
    Boolean isStatic;
    union
    {
        struct
        {
            List parameterList;
            List statementList;
        } cria;
        struct
        {
            CriaNativeFunction* function;
        } native;
    } of;
};



struct DefinitionClassTag
{
    String name;
    Boolean isNative;
    List baseList;
    Hash i_fields;
    Hash s_fields;
    Hash i_methods;
    Hash s_methods;
};



#endif
