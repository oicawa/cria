#ifndef PUBLIC_DEFINITIONCLASS_H_INCLUDED
#define PUBLIC_DEFINITIONCLASS_H_INCLUDED



#include "Interpreter.h"
#include "String.h"
#include "Boolean.h"
#include "List.h"
#include "CriaId.h"



typedef struct DefinitionClassTag *DefinitionClass;



typedef DefinitionClass CriaNativeClassLoader(Interpreter interpreter, char* name);



DefinitionClass
DefinitionClass_new(
	Interpreter interpreter,
    String name,
    Boolean isNative,
    List fieldList,
    List methodList,
    CriaNativeClassLoader* classLoader
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
    DefinitionClass klass,
    List parameters
);



List
DefinitionClass_getMethods(
	DefinitionClass klass
);



#endif
