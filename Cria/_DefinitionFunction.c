#include "_Cria.h"

FunctionDefinition
functionDefinition_new(
    char*               name,
    Boolean             isNative,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
)
{
    FunctionDefinition definition = Memory_malloc(sizeof(struct FunctionDefinitionTag));
    memset(definition, 0x00, sizeof(struct FunctionDefinitionTag));
    definition->name = name;
    definition->isNative = isNative;
    if (isNative == TRUE)
    {
        Logger_dbg("Native Function");
        definition->of.native.function = nativeFunctionPoint;
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



List
functionDefinition_parseParameters(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
	List parameters = list_new();
	Token token = NULL;
	
	while (TRUE)
	{
		token = parser_getCurrent(parser);
		if (token->type != TOKEN_TYPE_IDENTIFIER)
	    {
	        Logger_dbg("Not identifier");
			break;
	    }
	    
		VariableDefinition variable = variableDefinition_new(token->buffer);
		list_add(parameters, variable);
	    Logger_dbg("Add parameter.(%s)", variable->name->pointer);
		
		parser_next(parser);
		token = parser_getCurrent(parser);
		if (token->type != TOKEN_TYPE_COMMA)
	    {
	        Logger_dbg("Not ', '");
			break;
	    }
	    
		parser_next(parser);
	}
	
    Logger_dbg("parameters count = %d", parameters->count);
    Logger_trc("[  END  ]%s", __func__);
	return parameters;
}



FunctionDefinition
functionDefinition_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    FunctionDefinition functionDefinition = NULL;
    Item restore = parser_getPosition(parser);
    String name = NULL;
    List parameters = NULL;
    List statements = NULL;
    Statement statement = NULL;
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = token->buffer;
    parser_next(parser);
    
    if (parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, FALSE) == FALSE)
    {
        Logger_dbg("Not '('.");
    	goto END;
    }
    
	parameters = functionDefinition_parseParameters(parser);
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
        if (token->type == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = statement_parse(parser);
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
	
    Logger_dbg("Create FunctionDefinition. (name=%s, parameter=%p)", name->pointer, parameters);
    functionDefinition = functionDefinition_new(name->pointer, FALSE, parameters, statements, NULL);
    
END:
	if (functionDefinition == NULL)
		parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return functionDefinition;
}



CriaId
functionDefinition_evaluate(
	Interpreter interpreter,
	List parameterList,
	FunctionDefinition function,
	List parameters
)
{
    Logger_trc("[ START ]%s", __func__);
    int i = 0;
    CriaId value = NULL;
    VariableDefinition definition = NULL;
    
    Logger_dbg("function is %p", function);
    Logger_dbg("function->name is '%s'", function->name);
    Logger_dbg("function->of.cria.parameterList is %p", function->of.cria.parameterList);
    Logger_dbg("function->of.cria.parameterList->count is %d", function->of.cria.parameterList->count);
    Logger_dbg("parameters->count is %d", parameters->count);
    
    List list = function->of.cria.parameterList;
    Item item = list->item;
    Logger_dbg("item is %p", item);
    while (item != NULL)
    {
        Logger_dbg("item->object is %p", item->object);
        VariableDefinition def = (VariableDefinition)(item->object);
        Logger_dbg("Cast to VariableDefinition(item->object)");
        Logger_dbg("def->name is %p", def->name);
        Logger_dbg("def->name->pointer is %p", def->name->pointer);
        item = item->next;
    }
    Logger_dbg("Loop end.");
    
    
    //パラメータ数のチェック
    if (function->of.cria.parameterList->count != parameters->count)
    	runtime_error(interpreter);
    
    //パラメータをセット
    for (i = 0; i < parameters->count; i++)
    {
    	value = (CriaId)list_get(parameters, i);
    	definition = (VariableDefinition)list_get(function->of.cria.parameterList, i);
    	definition->object = value;
    }
    
    //実行
	executor_executeStatementList(interpreter, parameterList, function->of.cria.statementList);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return value;
}
