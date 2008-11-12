#include <stdio.h>

#include "_Cria.h"



//========================================
//Statement
//========================================
//Statement生成
Statement
statement_new(
    StatementKind   kind
)
{
    Statement statement = Memory_malloc(sizeof(struct StatementTag));
    memset(statement, 0x00, sizeof(struct StatementTag));
    statement->kind = kind;
    return statement;
}



//Statement破棄
void
statement_dispose(
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    if (statement == NULL)
    {
        goto END;
    }
    
    switch (statement->kind)
    {
    case STATEMENT_KIND_RETURN:
        //returnStatement_dispose(statement->of._return_);
        break;
    case STATEMENT_KIND_CATCH:
        //finallyStatement_dispose(statement->of._catch_);
        break;
    case STATEMENT_KIND_FINALLY:
        //finallyStatement_dispose(statement->of._finally_);
        break;
    case STATEMENT_KIND_IF:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_WHILE:
        //whileStatement_dispose(statement->of._while_);
        break;
    case STATEMENT_KIND_FOR:
        //forStatement_dispose(statement->of._for_);
        break;
    default:
        Logger_err("Illegal statement type. (%d)", statement->kind);
        break;
    }
    Memory_free(statement);
    statement = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



Statement
statement_parseSubstitute(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    SubstituteStatement substitute = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    Reference reference = NULL;
    Expression expression = NULL;
    
    
    //左辺の式（参照）を取得
    Logger_dbg("Check reference expression.");
    reference = reference_parse(parser);
    if (reference == NULL)
    {
        Logger_dbg("Not reference expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    //代入トークンのチェック
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_SUBSTITUTE)
    {
        Logger_dbg("Not substitution token.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    //以降のトークンを式としてパース
    Logger_dbg("Check 'Expression'");
    parser_next(parser);
    expression = expression_parse(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        Logger_dbg("Not new line token.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    //次のパースの為にカーソルを進める
    parser_next(parser);
    
    
    //生成
    Logger_dbg("Create 'SubstituteStatement'");
    substitute = Memory_malloc(sizeof(struct SubstituteStatementTag));
    memset(substitute, 0x00, sizeof(struct SubstituteStatementTag));
    substitute->reference = reference;
    substitute->expression = expression;
    
    statement = statement_new(STATEMENT_KIND_SUBSTITUTE);
    statement->of._substitute_ = substitute;
    statement->line = token->row;
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}




Statement
statement_parseFunctionCall(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    FunctionCallStatement  function = NULL;
    ReferenceExpression expression = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    
    //関数呼び出し式
    Logger_dbg("Check 'FunctionCallExpression'");
    expression = expression_parseReferenceExpression(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not reference expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    //改行でなければNULL返却
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        Logger_dbg("Not new line token.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    //次のパースの為にカーソルを進める
    parser_next(parser);
    
    
    //生成
    Logger_dbg("Create 'FunctionCallExpression'");
    function = Memory_malloc(sizeof(struct FunctionCallStatementTag));
    memset(function, 0x00, sizeof(struct FunctionCallStatementTag));
    function->expression = expression;
    
    statement = statement_new(STATEMENT_KIND_FUNCTION_CALL);
    statement->of._functionCall_ = function;
    statement->line = token->row;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



Statement
statement_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    
    statement = statement_parseSubstitute(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created SubstituteStatement.");
        goto END;
    }
    
    
    statement = statement_parseFunctionCall(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created FunctionCallStatement.");
        goto END;
    }
    
    /*
    //ReturnStatement?
    ReturnStatement returnStatement = returnStatement_parse(buffer, offset);
    if (returnStatement != NULL)
    {
        statement = Statement_new(STATEMENT_KIND_RETURN);
        statement->of._return_ = returnStatement;
        return statement;
    }
    //*/
    /*
    //WhileStatement
    WhileStatement whileStatement = whileStatement_parse(interpreter, buffer, offset);
    if (whileStatement != NULL)
    {
        statement = Statement_new(STATEMENT_KIND_WHILE);
        statement->of._while_ = whileStatement;
        return statement;
    }
    //*/
    
    /*
    //IfStatement?
    IfStatement ifStatement = ifStatement_parse(buffer, offset);
    if (ifStatement != NULL)
    {
        statement = Statement_new(STATEMENT_KIND_IF);
        statement->of._if_ = ifStatement;
        return statement;
    }
    //*/
    /*
    //代入式
    Logger_dbg("Check 'SubstituteStatement'");
    token_log(((Token)(mark->object)));
    parser_returnToMark(parser, mark);
    SubstituteStatement substituteStatement = NULL;
    substituteStatement = substituteStatement_parse(parser);
    if (substituteStatement != NULL)
    {
        Logger_dbg("Create 'FunctionCallExpression'");
        statement = statement_new(STATEMENT_KIND_SUBSTITUTE);
        statement->of._substitute_ = substituteStatement;
        goto END;
    }
    
    
    //関数呼び出し文
    Logger_dbg("Check 'FunctionCallStatement'");
    token_log(((Token)(mark->object)));
    parser_returnToMark(parser, mark);
    FunctionCallStatement functionCallStatement = NULL;
    functionCallStatement = functionCallStatement_parse(parser);
    if (functionCallStatement != NULL)
    {
        Logger_dbg("Create 'FunctionCallStatement'");
        statement = statement_new(STATEMENT_KIND_FUNCTION_CALL);
        statement->of._functionCall_ = functionCallStatement;
        goto END;
    }
    */
    
    Logger_dbg("No Statement.");
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}












































