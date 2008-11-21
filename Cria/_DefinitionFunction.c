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
	List parameters = NULL;
	
	
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
    Item position = parser_getPosition(parser);
    String name = NULL;
    List parameters = NULL;
    List statements = NULL;
    Statement statement = NULL;
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
    	parser_setPosition(parser, position);
    	goto END;
    }
    name = token->buffer;

	parser_next(parser);    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
    	parser_setPosition(parser, position);
    	goto END;
    }
    
	parser_next(parser);
	parameters = functionDefinition_parseParameters(parser);
	if (parameters == NULL)
	{
    	parser_setPosition(parser, position);
    	goto END;
	}
	
	token = parser_getCurrent(parser);
	if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
	{
    	parser_setPosition(parser, position);
    	goto END;
	}
	
	parser_next(parser);
	token = parser_getCurrent(parser);
	if (token->type != TOKEN_TYPE_COLON)
	{
    	parser_setPosition(parser, position);
    	goto END;
	}
    
	parser_next(parser);
	token = parser_getCurrent(parser);
	if (token->type != TOKEN_TYPE_NEW_LINE)
	{
    	parser_setPosition(parser, position);
    	goto END;
	}
    
	parser_next(parser);
	token = parser_getCurrent(parser);
	if (token->type != TOKEN_TYPE_INDENT)
	{
    	parser_setPosition(parser, position);
    	goto END;
	}
	
	parser_next(parser);
	statements = list_new();
    while(1)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token->type == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is not 'DEDENT'.");
            break;
        }
        
        statement = statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
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
    Logger_trc("[  END  ]%s", __func__);
    return functionDefinition;
}
