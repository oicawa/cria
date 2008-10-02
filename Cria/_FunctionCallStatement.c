#include <stdio.h>

#include "_Cria.h"



FunctionCallStatement
functionCallStatement_new(
    FunctionCallExpression expression
)
{
    FunctionCallStatement statement = NULL;
    statement = Memory_malloc(sizeof(struct FunctionCallStatementTag));
    memset(statement, 0x00, sizeof(struct FunctionCallStatementTag));
    statement->expression = expression;
    return statement;
}



void
functionCallStatement_dispose(
    FunctionCallStatement statement
)
{
    Logger_trc("[ START ]%s", __func__);
    if (statement == NULL)
    {
        Logger_dbg("statement is NULL.");
        goto END;
    }
    
    if (statement->expression != NULL)
    {
        functionCallExpression_dispose(statement->expression);
        statement->expression = NULL;
    }
    
    Memory_free(statement);
    statement = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



FunctionCallStatement
functionCallStatement_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    FunctionCallStatement  functionCallStatement = NULL;
    Token token = NULL;
    
    
    //関数呼び出し式
    Logger_dbg("Check 'FunctionCallExpression'");
    FunctionCallExpression functionCallExpression = NULL;
    functionCallExpression = functionCallExpression_parse(parser);
    if (functionCallExpression == NULL)
    {
        Logger_dbg("Not function call expression.");
        goto END;
    }
    
    
    //次のトークンをチェック
    //次のトークンがなければNULLを返却
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Next token does not exist.");
        goto END;
    }
    
    
    //あればそれを取得
    //現在のトークンを取得
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    
    
    //改行でなければNULL返却
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        Logger_dbg("It is not new line token.");
        goto END;
    }
    
    
    //関数呼び出し文生成
    Logger_dbg("Create 'FunctionCallExpression'");
    functionCallStatement = functionCallStatement_new(functionCallExpression);
    
END:
    token_log(token);
    Logger_trc("[  END  ]%s", __func__);
    return functionCallStatement;
}
    

