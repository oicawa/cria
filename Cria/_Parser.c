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
    parser->turningPoint = NULL;
    
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
    token_log((Token)(current->object));
    parser->current = current;
    parser->next = current->next;
    return TRUE;
}



Token
parser_getCurrent(
    Parser  parser
)
{
    return (Token)(parser->current->object);
}



Token
parser_getNext(
    Parser  parser
)
{
    return (Token)(parser->next->object);
}



void
returnToTurningPoint(
    Parser  parser,
    Item    turningPoint
)
{
    parser->current = turningPoint;
    parser->next = turningPoint->next;
}



void
setNewTurningPoint(
    Parser  parser
)
{
    parser->turningPoint = parser->current;
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
    Token turningPoint = NULL;
    Statement statement = NULL;
    
    
    //次のトークンがある間はループ
    Logger_dbg("Loop start.");
    while (parser_next(parser) == TRUE)
    {
        turningPoint = parser_getCurrent(parser);
        
        //ステートメント        
        Logger_dbg("Check statement.");
        statement = statement_parse(parser);
        if (statement != NULL)
        {
            Logger_dbg("Add created statement and parse next.");
            //statement->line = ((Token)(turningPoint->object))->row;
            addStatement(interpreter, statement);
            Logger_dbg("Set new turning point.");
            setNewTurningPoint(parser);
            continue;
        }
        
        
        /*
        //関数宣言文
        if (parseFunctionDefinition(parser) == TRUE)
        {
            setTurningPoint(parser);
            continue;
        }
        returnToTurningPoint(turningPoint);
        
        
        //クラス宣言文
        if (parseClassDefinition(parser) == TRUE)
        {
            setTurningPoint(parser);
            continue;
        }
        returnToTurningPoint(turningPoint);
        //*/
        
        //何れにも当てはまらない場合はエラー扱い
        perror("Syntax error.\n");
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










































