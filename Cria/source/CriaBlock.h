#ifndef PUBLIC_CRIABLOCK_H_INCLUDED
#define PUBLIC_CRIABLOCK_H_INCLUDED



#include "Cria.h"
#include "CriaId.h"
#include "List.h"
#include "Definition.h"
#include "Expression.h"
#include "Interpreter.h"





struct CriaBlockTag
{
    struct CriaIdTag id;
    Interpreter interpreter;
    CriaId object;
    List parameterList;
    DefinitionFunction function;
};



CriaBlock
CriaBlock_new(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    DefinitionFunction function
);



#endif
