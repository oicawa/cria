#include "Memory.h"
#include "Logger.h"

#include "String.h"
#include "Definition.h"
#include "Interpreter.h"


#include "_CriaBlock.h"



CriaBlock
CriaBlock_new(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    List parameters,
    DefinitionFunction function,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBlock block = NULL;
    
    block = Memory_malloc(sizeof(struct CriaBlockTag));
    block->id.name = String_clone("<<block>>");
    block->id.type = CRIA_DATA_TYPE_BLOCK;
    block->interpreter = interpreter;
    block->object = object;
    block->parameterList = parameterList;
    block->parameters = parameters;
    block->function = function;
    block->parent = parent;
    
    Logger_trc("[  END  ]%s", __func__);
    return block;
}


CriaId
CriaBlock_evaluate(
    CriaBlock block
)
{
    CriaId id = NULL;
    Interpreter interpreter = block->interpreter;
    CriaId object = block->object;
    List parameterList = block->parameterList;
    List parameters = block->parameters;
    DefinitionFunction function = block->function;
    CriaId parent = block->parent;
    
    id = DefinitionFunction_evaluate(interpreter, object, parameterList, function, parameters, parent);
    
    return id;
}

