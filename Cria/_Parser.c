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
parser_returnToMark(
    Parser  parser,
    Item    mark
)
{
    Logger_trc("[ START ]%s", __func__);
    if (parser == NULL)
        return;
    
    parser->current = mark;
    
    if (mark == NULL)
        parser->next = parser->tokens->item;
    else
        parser->next = mark->next;
    
    Logger_trc("[  END  ]%s", __func__);
}



void
setNewTurningPoint(
    Parser  parser
)
{
    parser->mark = parser->current;
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
parser_parse(
    Parser      parser,
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    Boolean result = FALSE;
    Item mark = NULL;
    Statement statement = NULL;
    Token errorToken = NULL;
    
    
    //次のトークンがある間はループ
    Logger_dbg("Loop start.");
    parser_next(parser);
    while (1)
    {
        setNewTurningPoint(parser);
        mark = parser->mark;
        
        
        //クラス定義？
        if (classDefinition_isMatch(parser) == TRUE)
        {
            Logger_dbg("Parse class definition.");
            continue;
        }
        
        
        //関数定義？
        if (functionDefinition_isMatch(parser) == TRUE)
        {
            Logger_dbg("Parse function definition.");
            continue;
        }
        
        
        //ステートメント        
        Logger_dbg("Check statement.");
        statement = statement_parse(parser);
        if (statement != NULL)
        {
            Logger_dbg("Add created statement and parse next.");
            addStatement(interpreter, statement);
            Logger_dbg("Set new turning point.");
            continue;
        }
        else
        {
            //最後のトークンまで来た。
            break;
        }
        
        
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









































