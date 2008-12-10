#ifndef PUBLIC_DEFINITIONFUNCTION_H_INCLUDED
#define PUBLIC_DEFINITIONFUNCTION_H_INCLUDED


#include "Interpreter.h"
#include "List.h"
#include "CriaId.h"
#include "Parser.h"



typedef CriaId CriaNativeFunction(Interpreter interpreter, CriaId object, List args);
typedef struct DefinitionFunctionTag *DefinitionFunction;



DefinitionFunction
DefinitionFunction_new(
    char*               name,
    Boolean             isNative,
    Boolean             isStatic,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
);



DefinitionFunction
DefinitionFunction_parse(
    Parser parser
);



DefinitionFunction
DefinitionFunction_search(
    List    functions,
    char*   name
);



CriaId
DefinitionFunction_evaluate(
	Interpreter interpreter,
    CriaId id,
	List parameterList,
	DefinitionFunction function,
	List parameters
);



List
DefinitionFunction_getParameterList(
	DefinitionFunction function
);



#endif
