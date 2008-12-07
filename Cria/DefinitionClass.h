#ifndef PUBLIC_DEFINITIONCLASS_H_INCLUDED
#define PUBLIC_DEFINITIONCLASS_H_INCLUDED


#include "Boolean.h"
#include "String.h"
#include "List.h"
#include "Interpreter.h"
#include "CriaId.h"



typedef struct DefinitionClassTag *DefinitionClass;



typedef DefinitionClass CriaNativeClassLoader(Interpreter interpreter, char* name);



DefinitionClass
definition_class_new(
	Interpreter interpreter,
    char*               name,
    Boolean             isNative,
    List                fieldList,
    List                methodList,
    CriaNativeClassLoader* classLoader
);



DefinitionClass
definition_class_search(
    List classList,
    char* name
);



CriaId
definition_class_evaluate(
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
