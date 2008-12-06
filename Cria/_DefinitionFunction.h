#ifndef PRIVATE_DEFINITIONFUNCTION_H_INCLUDED
#define PRIVATE_DEFINITIONFUNCTION_H_INCLUDED


#include "Interpreter.h"
#include "List.h"
#include "CriaId.h"
#include "Boolean.h"



typedef CriaId CriaNativeFunction(Interpreter interpreter, CriaId object, List args);



typedef struct DefinitionFunctionTag *DefinitionFunction;
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



DefinitionFunction
definition_function_new(
    char*               name,
    Boolean             isNative,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
);



DefinitionFunction
definition_function_search(
    List    functions,
    char*   name
);



CriaId
definition_function_evaluate(
	Interpreter interpreter,
    CriaId id,
	List parameterList,
	DefinitionFunction function,
	List parameters
);



#endif
