#ifndef PRIVATE_DEFINITIONCLASS_H_INCLUDED
#define PRIVATE_DEFINITIONCLASS_H_INCLUDED


#include "_Boolean.h"
#include "_String.h"
#include "_List.h"
#include "_Interpreter.h"
#include "_CriaId.h"



typedef struct DefinitionClassTag *DefinitionClass;
struct DefinitionClassTag
{
    String      name;
    Boolean     isNative;
    List        baseList;
    List        fieldList;
    List        methodList;
};



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



#endif
