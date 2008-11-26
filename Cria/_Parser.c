#include <stdio.h>
#include <ctype.h>

#include "_Cria.h"



Parser
parser_new(
    List    list
)
{
    Logger_trc("[ START ]%s", __func__);
    Parser parser = Memory_malloc(sizeof(struct ParserTag));
    memset(parser, 0x00, sizeof(struct ParserTag));
    
    parser->tokens = list;
    parser->current = NULL;
    parser->next = list->item;
    parser->mark = NULL;
    
    Logger_trc("[  END  ]%s", __func__);
    return parser;
}



void
parser_dispose(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Logger_dbg("Check parser.");
    if (parser == NULL)
    {
        Logger_dbg("parser is NULL.");
        goto END;
    }
    
    /*
    Logger_dbg("Check parser->tokens.");
    if (parser->tokens != NULL)
    {
        Logger_dbg("Free parser->tokens.");
        List_dispose(parser->tokens);
        Logger_dbg("Set NULL to parser->tokens.");
        parser->tokens = NULL;
    }
    */
    
    Logger_dbg("Free parser.");
    Memory_free(parser);
    Logger_dbg("Set NULL to parser.");
    parser = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
parser_next(
    Parser  parser
)
{
    if (parser == NULL)
    {
        Logger_dbg("Parser is NULL.");
        return FALSE;
    }
    
    
    if (parser->next == NULL)
    {
        Logger_dbg("Next token is  NULL.");
        return FALSE;
    }
    
    
    Item current = parser->next;
    token_log(((Token)(current->object)));
    parser->current = current;
    parser->next = current->next;
    
    return TRUE;
}



Token
parser_getCurrent(
    Parser  parser
)
{
    Logger_dbg("[ START ]%s", __func__);
    if (parser == NULL)
    {
        Logger_dbg("parser is NULL.");
        return NULL;
    }
    Logger_dbg("parser is not NULL.");
    Item item = parser->current;
    if (item == NULL)
    {
        Logger_dbg("item is NULL.");
        return NULL;
    }
    Logger_dbg("item is not NULL.");
    Token token = (Token)(item->object);
    if (token == NULL)
    {
        Logger_dbg("token is NULL.");
        return NULL;
    }
    Logger_dbg("token is not NULL.");
    
    Logger_dbg("[  END  ]%s", __func__);
    return token;
}



Token
parser_getNext(
    Parser  parser
)
{
    Logger_dbg("[ START ]%s", __func__);
    if (parser == NULL)
    {
        Logger_dbg("parser is NULL.");
        return NULL;
    }
    Logger_dbg("parser is not NULL.");
    Item item = parser->next;
    if (item == NULL)
    {
        Logger_dbg("item is NULL.");
        return NULL;
    }
    Logger_dbg("item is not NULL.");
    Token token = (Token)(item->object);
    if (token == NULL)
    {
        Logger_dbg("token is NULL.");
        return NULL;
    }
    Logger_dbg("token is not NULL.");
    
    Logger_dbg("[  END  ]%s", __func__);
    return token;
}



Item
parser_getPosition(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    if (parser == NULL)
        return NULL;
    
    Logger_trc("[  END  ]%s", __func__);
    return parser->current;
}



void
parser_setPosition(
    Parser  parser,
    Item    position
)
{
    Logger_trc("[ START ]%s", __func__);
    if (parser == NULL)
        return;
    
    parser->current = position;
    
    if (position == NULL)
        parser->next = parser->tokens->item;
    else
        parser->next = position->next;
    
    Logger_trc("[  END  ]%s", __func__);
}




void
addStatement(
    Interpreter interpreter,
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    Logger_trc("statement is [%p]", statement);
    list_add(interpreter->statementList, statement);
    Logger_trc("[  END  ]%s", __func__);
}


Boolean
parser_eat(
	Parser parser,
	TokenType type,
	Boolean isNessesally
)
{
	Boolean result = FALSE;
	Token token = parser_getCurrent(parser);
	if (token == NULL)
		goto UNMATCH;
	
	if (token->type != type)
		goto UNMATCH;
	
	result = TRUE;
	parser_next(parser);
	goto END;

UNMATCH:
	if (isNessesally == TRUE)
		parser_error(token);
	
END:
	return result;
}



Boolean
parser_isEnd(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = NULL;
    Boolean result = FALSE;
    
    while (TRUE)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token == NULL)
        {
            result = TRUE;
            goto END;
        }
        
        if (token->type != TOKEN_TYPE_NEW_LINE)
            goto END;
        
        if (parser_next(parser) == FALSE)
        {
            result = TRUE;
            break;
        }
    }
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
parser_parse(
    Parser      parser,
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    Boolean result = FALSE;
    Statement statement = NULL;
    FunctionDefinition functionDefinition = NULL;
    Token errorToken = NULL;
    
    
    //次のトークンがある間はループ
    Logger_dbg("Loop start.");
    parser_next(parser);
    while (TRUE)
    {
        functionDefinition = functionDefinition_parse(parser);
        if (functionDefinition != NULL)
        {
            Logger_dbg("Add created FunctionDefinition and parse next.");
            list_add(interpreter->functionList, functionDefinition);
            continue;
        }
        
        //ステートメント        
        Logger_dbg("Check statement.");
        statement = statement_parse(parser);
        if (statement != NULL)
        {
            Logger_dbg("Add created statement and parse next.");
            addStatement(interpreter, statement);
            continue;
        }
        
        if (parser_isEnd(parser) == TRUE)
            break;
        
        //何れにも当てはまらない場合はエラー扱い
        errorToken = parser_getCurrent(parser);
        parser_error(errorToken);
        goto END;
    }
    
    Logger_dbg("Loop end.");
    
    
    //処理成功
    Logger_dbg("Parse Successed.");
    result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}


