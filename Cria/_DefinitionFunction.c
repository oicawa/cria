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
#include "Parser.h"

#include "_DefinitionFunction.h"


DefinitionFunction
DefinitionFunction_new(
    char*               name,
    Boolean             isNative,
    Boolean             isStatic,
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
        Logger_dbg("Cria Function (ParameterCount=%d, StatementCount=%d)", List_count(parameterList), List_count(statementList));
        definition->of.cria.parameterList = parameterList;
        definition->of.cria.statementList = statementList;
    }

    return definition;
}



List
DefinitionFunction_parse_parameters(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
	List parameters = List_new();
	Token token = NULL;
	
	while (TRUE)
	{
		token = Parser_getCurrent(parser);
		if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
	    {
	        Logger_dbg("Not identifier");
			break;
	    }
	    
		DefinitionVariable variable = DefinitionVariable_new(Token_buffer(token));
		List_add(parameters, variable);
	    Logger_dbg("Add parameter.(%s)", DefinitionVariable_name(variable));
		
		Parser_next(parser);
		token = Parser_getCurrent(parser);
		if (Token_type(token) != TOKEN_TYPE_COMMA)
	    {
	        Logger_dbg("Not ', '");
			break;
	    }
	    
		Parser_next(parser);
	}
	
    Logger_dbg("parameters count = %d", List_count(parameters));
    Logger_trc("[  END  ]%s", __func__);
	return parameters;
}



DefinitionFunction
DefinitionFunction_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction functionDefinition = NULL;
    Item restore = Parser_getPosition(parser);
    String name = NULL;
    List parameters = NULL;
    List statements = NULL;
    Statement statement = NULL;
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = Token_buffer(token);
    Parser_next(parser);
    
    if (Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, FALSE) == FALSE)
    {
        Logger_dbg("Not '('.");
    	goto END;
    }
    
	parameters = DefinitionFunction_parse_parameters(parser);
	if (parameters == NULL)
    {
        Logger_dbg("Not parameters.");
    	goto END;
    }
	
	if (Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_RIGHT, FALSE) == FALSE)
    {
        Logger_dbg("Not ')'.");
    	goto END;
    }
	
	if (Parser_eat(parser, TOKEN_TYPE_COLON, FALSE) == FALSE)
    {
        Logger_dbg("Not ':'.");
    	goto END;
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_NEW_LINE, TRUE) == FALSE)
    {
        Logger_dbg("Not <<NEW LINE>>.");
    	goto END;
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_INDENT, TRUE) == FALSE)
    {
        Logger_dbg("Not <<INDENT>>.");
    	goto END;
    }
	
	statements = List_new();
    while(1)
    {
        token = Parser_getCurrent(parser);
        if (Token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            List_dispose(statements);
            Parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        List_add(statements, statement);
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_DEDENT, TRUE) == FALSE)
    {
        Logger_dbg("Not <<DEDENT>>.");
    	goto END;
    }
	
    Logger_dbg("Create FunctionDefinition. (name=%s, parameter=%p)", name, parameters);
    functionDefinition = DefinitionFunction_new(name, FALSE, TRUE, parameters, statements, NULL);
    
END:
	if (functionDefinition == NULL)
		Parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return functionDefinition;
}



DefinitionFunction
DefinitionFunction_search(
    List    functions,
    char*   name
)
{
    int count = List_count(functions);
    int index = 0;
    DefinitionFunction definition = NULL;
    DefinitionFunction tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionFunction)(List_get(functions, index));
        if (strcmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
DefinitionFunction_evaluate(
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
    Logger_dbg("function->of.cria.parameterList->count is %d", List_count(function->of.cria.parameterList));
    Logger_dbg("parameters->count is %d", List_count(parameters));
    
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
    if (List_count(function->of.cria.parameterList) != List_count(parameters))
    	runtime_error(interpreter);
    
    //パラメータをセット
    for (i = 0; i < List_count(parameters); i++)
    {
    	id = (CriaId)List_get(parameters, i);
    	definition = (DefinitionVariable)List_get(function->of.cria.parameterList, i);
    	DefinitionVariable_set(definition, id);
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
