#include "_Cria.h"

FunctionDefinition
functionDefinition_new(
    char*               name,
    Boolean             isNative,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* loader
)
{
    FunctionDefinition definition = Memory_malloc(sizeof(struct FunctionDefinitionTag));
    memset(definition, 0x00, sizeof(struct FunctionDefinitionTag));
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



FunctionDefinition
functionDefinition_search(
    List    functions,
    char*   name
)
{
    int count = functions->count;
    int index = 0;
    FunctionDefinition definition = NULL;
    FunctionDefinition tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (FunctionDefinition)(list_get(functions, index));
        if (strcmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
functionDefinition_evaluate(
	Interpreter interpreter,
    CriaId object,
	List parameterList,
	FunctionDefinition function,
	List parameters
)
{
    Logger_trc("[ START ]%s", __func__);
    int i = 0;
    CriaId id = NULL;
    StatementResult result;
    VariableDefinition definition = NULL;
    
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
    	definition = (VariableDefinition)list_get(function->of.cria.parameterList, i);
    	definition->object = id;
    }
    
    //実行
	result = executor_executeStatementList(interpreter, object, parameterList, function->of.cria.statementList);
    id = result.returns.id;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



