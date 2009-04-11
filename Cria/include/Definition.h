#ifndef PUBLIC_DEFINITION_H_INCLUDED
#define PUBLIC_DEFINITION_H_INCLUDED


#include "Interpreter.h"
#include "List.h"
#include "CriaId.h"
#include "Parser.h"
#include "String.h"
#include "Boolean.h"



typedef enum 
{
    DEFINITION_VARIABLE_TYPE_NORMAL,
    DEFINITION_VARIABLE_TYPE_ITEM,
} DefinitionVariableType;



typedef struct DefinitionVariableTag *DefinitionVariable;
typedef struct DefinitionVariableNormalTag *DefinitionVariableNormal;

typedef CriaId CriaNativeFunction(Interpreter interpreter, CriaId object, List args);
typedef struct DefinitionFunctionTag *DefinitionFunction;

typedef struct DefinitionClassTag *DefinitionClass;
typedef DefinitionClass CriaNativeClassLoader(char* name);



//==============================
//DefinitionVariable
//==============================
DefinitionVariable
DefinitionVariable_new(
    DefinitionVariableType type,
    String name,
    Boolean isStatic,
    Boolean isConstant,
    Item item
);



DefinitionVariable
DefinitionVariable_search(
    List parameters,
    String name
);



String
DefinitionVariable_name(
	DefinitionVariable variable
);



void
DefinitionVariable_set(
	DefinitionVariable variable,
	CriaId id
);



void*
DefinitionVariable_getObject(
	DefinitionVariable variable
);



DefinitionVariable
DefinitionVariable_parse(
	Parser parser
);



//==============================
//DefinitionFunction
//==============================
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



String
DefinitionFunction_get_name(
	DefinitionFunction function
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
	List parameters,
	CriaId parent
);



List
DefinitionFunction_getParameterList(
	DefinitionFunction function
);



//==============================
//DefinitionClass
//==============================
DefinitionClass
DefinitionClass_new(
    String name,
    Boolean isNative,
    Hash i_fields,
    Hash s_fields,
    Hash i_methods,
    Hash s_methods,
    CriaNativeClassLoader* classLoader
);



String
DefinitionClass_getName(
	DefinitionClass klass
);



DefinitionClass
DefinitionClass_search(
    List classList,
    char* name
);



CriaId
DefinitionClass_evaluate(
    Interpreter interpreter,
    CriaId  id,
    List parameterList,
    char*   name,
    Boolean isStatic,
    DefinitionClass klass,
    List parameters
);



Hash
DefinitionClass_getFields(
	DefinitionClass klass,
	Boolean isStatic
);



Hash
DefinitionClass_getMethods(
	DefinitionClass klass,
	Boolean isStatic
);



DefinitionClass
DefinitionClass_parse(
    Parser parser
);



CriaId
DefinitionClass_generateInstance(
    Interpreter interpreter,
    DefinitionClass klass,
    List parameters
);



#endif
