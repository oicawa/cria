#include "_Cria.h"

FunctionDefinition
functionDefinition_new(
    char*               name,
    Boolean             isNative,
    AccessLevel         access,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
)
{
    FunctionDefinition definition = Memory_malloc(sizeof(struct FunctionDefinitionTag));
    memset(definition, 0x00, sizeof(struct FunctionDefinitionTag));
    definition->name = name;
    definition->isNative = isNative;
    definition->access = access;
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



FunctionDefinition
functionDefinition_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    FunctionDefinition functionDefinition = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
    	goto END;
    }
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
    	parser_setPosition(parser, position);
    	parser_error(token);
    	goto END;
    }
	parser_next(parser);
    
    //生成
    Logger_dbg("Create 'GotoStatement'");
    gotoStatement = Memory_malloc(sizeof(struct GotoStatementTag));
    memset(gotoStatement, 0x00, sizeof(struct GotoStatementTag));
    gotoStatement->type = type;
    if (label != NULL)
    	gotoStatement->of.label = string_clone(label);
    gotoStatement->of.expression = expression;
    
    statement = statement_new(STATEMENT_KIND_GOTO);
    statement->of._goto_ = gotoStatement;
    statement->line = token->row;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}
