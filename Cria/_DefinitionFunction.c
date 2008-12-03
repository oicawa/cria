#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_Boolean.h"
#include "_List.h"
#include "_Interpreter.h"
#include "_CriaId.h"
#include "_Statement.h"
#include "_DefinitionVariable.h"
#include "_Runtime.h"
#include "_Executor.h"

#include "_DefinitionFunction.h"


DefinitionFunction
definition_function_new(
    char*               name,
    Boolean             isNative,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* loader
)
{
    DefinitionFunction definition = Memory_malloc(sizeof(struct DefinitionFunctionTag));
    memset(definition, 0x00, sizeof(struct DefinitionFunctionTag));
    definition->name = name;
    definition->isNative = isNative;
    definition->isStatic = TRUE;
    if (isNative == TRUE)
    {
        Logger_dbg("Native Function");
        definition->of.native.function = loader;
    }
    else
    {
        Logger_dbg("Cria Function (ParameterCount=%d, StatementCount=%d)", parameterList->count, statementList->count);
        definition->of.cria.parameterList = parameterList;
        definition->of.cria.statementList = statementList;
    }

    return definition;
}



DefinitionFunction
definition_function_search(
    List    functions,
    char*   name
)
{
    int count = functions->count;
    int index = 0;
    DefinitionFunction definition = NULL;
    DefinitionFunction tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionFunction)(list_get(functions, index));
        if (strcmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
definition_function_evaluate(
	Interpreter interpreter,
    CriaId object,
	List parameterList,
	DefinitionFunction function,
	List parameters
)
{
    Logger_trc("[ START ]%s", __func__);
    int i = 0;
    CriaId id = NULL;
    StatementResult result;
    DefinitionVariable definition = NULL;
    
    if (function->isNative == TRUE)
    {
        Logger_dbg("Call native function.(%s)", function->name);
        id = (*(function->of.native.function))(interpreter, object, parameters);
        goto END;
    }
    
    Logger_dbg("function is %p", function);
    Logger_dbg("function->name is '%s'", function->name);
    Logger_dbg("function->of.cria.parameterList is %p", function->of.cria.parameterList);
    Logger_dbg("function->of.cria.parameterList->count is %d", function->of.cria.parameterList->count);
    Logger_dbg("parameters->count is %d", parameters->count);
    
    //List list = function->of.cria.parameterList;
    //Item item = list->item;
    //Logger_dbg("item is %p", item);
    //while (item != NULL)
    //{
        //Logger_dbg("item->object is %p", item->object);
        //VariableDefinition def = (VariableDefinition)(item->object);
        //Logger_dbg("Cast to VariableDefinition(item->object)");
        //Logger_dbg("def->name is %p", def->name);
        //Logger_dbg("def->name->pointer is %p", def->name->pointer);
        //item = item->next;
    //}
    //Logger_dbg("Loop end.");
    
    
    //パラメータ数のチェック
    if (function->of.cria.parameterList->count != parameters->count)
    	runtime_error(interpreter);
    
    //パラメータをセット
    for (i = 0; i < parameters->count; i++)
    {
    	id = (CriaId)list_get(parameters, i);
    	definition = (DefinitionVariable)list_get(function->of.cria.parameterList, i);
    	definition->object = id;
    }
    
    //実行
	result = executor_executeStatementList(interpreter, object, parameterList, function->of.cria.statementList);
    id = result.returns.id;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



