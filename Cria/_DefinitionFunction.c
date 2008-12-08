#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "Boolean.h"
#include "List.h"
#include "Interpreter.h"
#include "CriaId.h"
#include "Statement.h"
#include "DefinitionVariable.h"
#include "Runtime.h"
#include "Executor.h"
#include "Parser.h"

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
        Logger_dbg("Cria Function (ParameterCount=%d, StatementCount=%d)", list_count(parameterList), list_count(statementList));
        definition->of.cria.parameterList = parameterList;
        definition->of.cria.statementList = statementList;
    }

    return definition;
}



List
definition_function_parse_parameters(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
	List parameters = list_new();
	Token token = NULL;
	
	while (TRUE)
	{
		token = parser_getCurrent(parser);
		if (token_type(token) != TOKEN_TYPE_IDENTIFIER)
	    {
	        Logger_dbg("Not identifier");
			break;
	    }
	    
		DefinitionVariable variable = definition_variable_new(token_buffer(token));
		list_add(parameters, variable);
	    Logger_dbg("Add parameter.(%s)", definition_variable_name(variable));
		
		parser_next(parser);
		token = parser_getCurrent(parser);
		if (token_type(token) != TOKEN_TYPE_COMMA)
	    {
	        Logger_dbg("Not ', '");
			break;
	    }
	    
		parser_next(parser);
	}
	
    Logger_dbg("parameters count = %d", list_count(parameters));
    Logger_trc("[  END  ]%s", __func__);
	return parameters;
}



DefinitionFunction
definition_function_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction functionDefinition = NULL;
    Item restore = parser_getPosition(parser);
    String name = NULL;
    List parameters = NULL;
    List statements = NULL;
    Statement statement = NULL;
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    if (token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = token_buffer(token);
    parser_next(parser);
    
    if (parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, FALSE) == FALSE)
    {
        Logger_dbg("Not '('.");
    	goto END;
    }
    
	parameters = definition_function_parse_parameters(parser);
	if (parameters == NULL)
    {
        Logger_dbg("Not parameters.");
    	goto END;
    }
	
	if (parser_eat(parser, TOKEN_TYPE_PARENTHESIS_RIGHT, FALSE) == FALSE)
    {
        Logger_dbg("Not ')'.");
    	goto END;
    }
	
	if (parser_eat(parser, TOKEN_TYPE_COLON, FALSE) == FALSE)
    {
        Logger_dbg("Not ':'.");
    	goto END;
    }
    
	if (parser_eat(parser, TOKEN_TYPE_NEW_LINE, TRUE) == FALSE)
    {
        Logger_dbg("Not <<NEW LINE>>.");
    	goto END;
    }
    
	if (parser_eat(parser, TOKEN_TYPE_INDENT, TRUE) == FALSE)
    {
        Logger_dbg("Not <<INDENT>>.");
    	goto END;
    }
	
	statements = list_new();
    while(1)
    {
        token = parser_getCurrent(parser);
        if (token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            list_dispose(statements);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    
	if (parser_eat(parser, TOKEN_TYPE_DEDENT, TRUE) == FALSE)
    {
        Logger_dbg("Not <<DEDENT>>.");
    	goto END;
    }
	
    Logger_dbg("Create FunctionDefinition. (name=%s, parameter=%p)", name, parameters);
    functionDefinition = definition_function_new(name, FALSE, parameters, statements, NULL);
    
END:
	if (functionDefinition == NULL)
		parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return functionDefinition;
}



DefinitionFunction
definition_function_search(
    List    functions,
    char*   name
)
{
    int count = list_count(functions);
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
    Logger_dbg("function->of.cria.parameterList->count is %d", list_count(function->of.cria.parameterList));
    Logger_dbg("parameters->count is %d", list_count(parameters));
    
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
    if (list_count(function->of.cria.parameterList) != list_count(parameters))
    	runtime_error(interpreter);
    
    //パラメータをセット
    for (i = 0; i < list_count(parameters); i++)
    {
    	id = (CriaId)list_get(parameters, i);
    	definition = (DefinitionVariable)list_get(function->of.cria.parameterList, i);
    	definition_variable_set(definition, id);
    }
    
    //実行
	result = Statement_executeList(interpreter, object, parameterList, function->of.cria.statementList);
    id = result.returns.id;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



List
DefinitionFunction_getParameterList(
	DefinitionFunction function
)
{
	return function->of.cria.parameterList;
}
