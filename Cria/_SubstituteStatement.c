#include <stdio.h>

#include "_Cria.h"



SubstituteStatement
substituteStatement_new(
    String      variable,
    Expression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    SubstituteStatement statement = NULL;
    statement = Memory_malloc(sizeof(struct SubstituteStatementTag));
    memset(statement, 0x00, sizeof(struct SubstituteStatementTag));
    statement->variable = variable;
    statement->expression = expression;
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



void
substituteStatement_dispose(
    SubstituteStatement statement
)
{
    Logger_trc("[ START ]%s", __func__);
    if (statement == NULL)
    {
        Logger_dbg("statement is NULL.");
        goto END;
    }
    
    if (statement->variable != NULL)
    {
        string_dispose(statement->variable);
        statement->variable = NULL;
    }
    
    if (statement->expression != NULL)
    {
        expression_dispose(statement->expression);
        statement->expression = NULL;
    }
    
    Memory_free(statement);
    statement = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



SubstituteStatement
substituteStatement_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    SubstituteStatement  substituteStatement = NULL;
    Token token = NULL;
    String variableName = NULL;
    Expression expression = NULL;
    
    //トークン取得
    token = parser_getCurrent(parser);
    
    
    //左辺の変数名を取得
    Logger_dbg("Check 'Variable Name'");
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("First token is not identifier.");
        goto END;
    }
    variableName = string_clone(token->buffer);
    
    
    //次のトークンが代入でなければ終了
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Next token does not exist.");
        goto END;
    }
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    if (token->type != TOKEN_TYPE_SUBSTITUTE)
    {
        Logger_dbg("It is not substitute token.");
        goto END;
    }
    
    
    //以降のトークンを式としてパース
    Logger_dbg("Check 'Expression'");
    expression = expression_parse(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not expression.");
        goto END;
    }
    
    
    Logger_dbg("Create 'SubstituteStatement'");
    substituteStatement = substituteStatement_new(variableName, expression);
    
END:
    token_log(token);
    Logger_trc("[  END  ]%s", __func__);
    return substituteStatement;
}



Boolean
substituteStatement_isMatch(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Boolean result = FALSE;
    Item mark = NULL;
    Token token = NULL;
    
    //NULLチェック
    if (parser == NULL)
    {
        goto END;
    }
    
    //現在のトークンを復帰位置としてバックアップ
    mark = parser->current;
    
    //改行トークンが検知される前に、代入演算子トークンが検知されればOK。
    while (parser_next(parser) == TRUE)
    {
        //改行トークン？
        token = parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_NEW_LINE)
        {
            goto END;
        }
        if (token->type == TOKEN_TYPE_SUBSTITUTE)
        {
            result = TRUE;
            break;
        }
    }
    
END:
    parser_returnToMark(parser, mark);
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



