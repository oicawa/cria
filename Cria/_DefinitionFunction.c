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
        definition->of.native.function = nativeFunctionPoint;
    }
    else
    {
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
			break;
		
		list_add(parameters, string_clone(token->buffer));
		
		parser_next(parser);
		token = parser_getCurrent(parser);
		if (token->type != TOKEN_TYPE_COMMA)
			break;
		
		parser_next(parser);
	}
	
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
    	goto END;
    
    name = token->buffer;
	
    if (parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, FALSE) == FALSE)
    	goto END;
    
	parameters = functionDefinition_parseParameters(parser);
	if (parameters == NULL)
    	goto END;
	
	if (parser_eat(parser, TOKEN_TYPE_PARENTHESIS_RIGHT, FALSE) == FALSE)
    	goto END;
	
	if (parser_eat(parser, TOKEN_TYPE_COLON, FALSE) == FALSE)
    	goto END;
    
	if (parser_eat(parser, TOKEN_TYPE_NEW_LINE, TRUE) == FALSE)
    	goto END;
    
	if (parser_eat(parser, TOKEN_TYPE_INDENT, TRUE) == FALSE)
    	goto END;
	
	statements = list_new();
    while(1)
    {
        token = parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is not 'DEDENT'.");
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
    
    //生成
    Logger_dbg("Create 'GotoStatement'");
    functionDefinition = functionDefinition_new(name->pointer, TRUE, parameters, statements, NULL);
    
END:
	if (functionDefinition == NULL)
		parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return functionDefinition;
}
