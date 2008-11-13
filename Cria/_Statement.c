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
    Logger_dbg("Create 'FunctionCallStatement'");
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



IfStatement
statement_parseElseBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    IfStatement ifStatement = NULL;
    List statements = list_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_ELSE)
    {
        Logger_err("token type is not 'ELSE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
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
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    //生成
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct IfStatementTag));
    memset(ifStatement, 0x00, sizeof(struct IfStatementTag));
    ifStatement->statements = statements;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



IfStatement
statement_parseElifBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    IfStatement ifStatement = NULL;
    Expression condition = NULL;
    List statements = list_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_ELIF)
    {
        Logger_err("token type is not 'ELIF'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    condition = expression_parse(parser);
    if (condition == NULL)
    {
        Logger_err("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
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
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    //生成
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct IfStatementTag));
    memset(ifStatement, 0x00, sizeof(struct IfStatementTag));
    ifStatement->condition = condition;
    ifStatement->statements = statements;
    
    
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_ELSE)
    {
        Logger_dbg("token type is 'ELSE'.");
        ifStatement->_if_ = statement_parseElseBlock(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



IfStatement
statement_parseIfBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    IfStatement ifStatement = NULL;
    Expression condition = NULL;
    List statements = list_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_IF)
    {
        Logger_dbg("token type is not 'IF'.");
        parser_setPosition(parser, position);
        goto END;
    }
    parser_next(parser);
    
    
    condition = expression_parse(parser);
    if (condition == NULL)
    {
        Logger_dbg("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token->type != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token->type == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    //生成
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct IfStatementTag));
    memset(ifStatement, 0x00, sizeof(struct IfStatementTag));
    ifStatement->condition = condition;
    ifStatement->statements = statements;
    
    
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_ELIF)
    {
        Logger_dbg("token type is 'ELIF'.");
        ifStatement->_if_ = statement_parseElifBlock(parser);
    }
    else if (token->type == TOKEN_TYPE_ELSE)
    {
        Logger_dbg("token type is 'ELSE'.");
        ifStatement->_if_ = statement_parseElseBlock(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



Statement
statement_parseIf(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    IfStatement ifStatement = NULL;
    Item position = parser_getPosition(parser);
    Token token = parser_getCurrent(parser);
    
    
    ifStatement = statement_parseIfBlock(parser);
    if (ifStatement == NULL)
    {
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    statement = statement_new(STATEMENT_KIND_IF);
    statement->of._if_ = ifStatement;
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
    
    
    statement = statement_parseIf(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created IfStatement.");
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
    
    
    Logger_dbg("No Statement.");
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}












































