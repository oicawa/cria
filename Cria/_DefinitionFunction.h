#ifndef PRIVATE_DEFINITIONFUNCTION_H_INCLUDED
#define PRIVATE_DEFINITIONFUNCTION_H_INCLUDED


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
    CriaId      object;
};



struct DefinitionFunctionTag
{
    char*                       name;
    Boolean                     isNative;
    Boolean                    isStatic;
    union
    {
        struct
        {
            List                parameterList;
            List                statementList;
        } cria;
        struct
        {
            CriaNativeFunction* function;
        } native;
    } of;
};



struct DefinitionClassTag
{
    String      name;
    Boolean     isNative;
    List        baseList;
    List        fieldList;
    List        methodList;
};



#endif
