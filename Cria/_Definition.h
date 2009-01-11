#ifndef PRIVATE_DEFINITION_H_INCLUDED
#define PRIVATE_DEFINITION_H_INCLUDED


#include "Interpreter.h"
#include "List.h"
#include "CriaId.h"
#include "Boolean.h"
#include "String.h"
#include "Definition.h"


struct DefinitionVariableTag
{
    String      name;
    Boolean     isStatic;
    void*       object;
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
    Hash fields;
    List methodList;
};



#endif
