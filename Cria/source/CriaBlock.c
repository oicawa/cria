#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "String.h"
#include "Definition.h"
#include "Interpreter.h"
#include "CriaBlock.h"



CriaBlock
CriaBlock_new(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    DefinitionFunction function
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBlock block = NULL;
    
    block = Memory_malloc(sizeof(struct CriaBlockTag));
    block->id.name = String_new("<<block>>");
    block->id.type = CRIA_DATA_TYPE_BLOCK;
    block->interpreter = interpreter;
    block->object = object;
    block->parameterList = parameterList;
    block->function = function;
    
    Logger_trc("[  END  ]%s", __func__);
    return block;
}


CriaId
CriaBlock_evaluate(
    CriaBlock block,
    List parameters
)
{
    CriaId id = NULL;
    Interpreter interpreter = block->interpreter;
    CriaId object = block->object;
    List parameterList = block->parameterList;
    DefinitionFunction function = block->function;
    
    id = DefinitionFunction_evaluate(interpreter, object, parameterList, NULL, function, parameters, NULL);
    
    return id;
}

