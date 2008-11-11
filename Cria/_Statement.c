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



Boolean
isMatchSubstituteStatement(
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



SubstituteStatement
parseSubstituteStatement(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    SubstituteStatement statement = NULL;
    Token token = NULL;
    Reference reference = NULL;
    Expression expression = NULL;
    
    
    //左辺の式（参照）を取得
    Logger_dbg("Check reference expression.");
    reference = reference_parse(parser);
    
    
    //代入トークンのチェック
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_SUBSTITUTE)
    {
        Logger_dbg("It is not substitute token.");
        goto END;
    }
    
    
    //以降のトークンを式としてパース
    Logger_dbg("Check 'Expression'");
    parser_next(parser);
    expression = expression_parse(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not expression.");
        goto END;
    }
    
    
    Logger_dbg("Create 'SubstituteStatement'");
    statement = Memory_malloc(sizeof(struct SubstituteStatementTag));
    memset(statement, 0x00, sizeof(struct SubstituteStatementTag));
    statement->reference = reference;
    statement->expression = expression;
    
END:
    token_log(token);
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}




FunctionCallStatement
parseFunctionCallStatement(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    FunctionCallStatement  functionCallStatement = NULL;
    ReferenceExpression expression = NULL;
    Token token = NULL;
    
    
    //関数呼び出し式
    Logger_dbg("Check 'FunctionCallExpression'");
    expression = expression_parseReferenceExpression(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not function call expression.");
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
        parser_error(token);
        goto END;
    }
    
    
    //関数呼び出し文生成
    Logger_dbg("Create 'FunctionCallExpression'");
    functionCallStatement = Memory_malloc(sizeof(struct FunctionCallStatementTag));
    memset(functionCallStatement, 0x00, sizeof(struct FunctionCallStatementTag));
    functionCallStatement->expression = expression;
    
END:
    //次のトークンへ移動
    parser_next(parser);
    
    
    token_log(parser_getCurrent(parser));
    Logger_trc("[  END  ]%s", __func__);
    return functionCallStatement;
}



Statement
statement_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    //Item mark = parser->mark;
    
    
    //行先頭のトークンをチェック。
    Token current = parser_getCurrent(parser);
    int line = current->row;
    switch (current->type)
    {
    case TOKEN_TYPE_IF:                     //if文
        break;
    case TOKEN_TYPE_FOR:                    //for文
        break;
    case TOKEN_TYPE_WHILE:                  //while文
        break;
    case TOKEN_TYPE_RETURN:                 //返却文（値なし）
        break;
    case TOKEN_TYPE_RETURN_VALUE:           //返却文（値あり）
        break;
    case TOKEN_TYPE_IDENTIFIER:             //変数参照（代入）／メソッド呼び出し
    case TOKEN_TYPE_CLASS_LITERAL:          //クラス参照（メンバ参照、メソッド実行）
        if (isMatchSubstituteStatement(parser) == TRUE)
        {
            Logger_dbg("Substitute statement parse.");
            SubstituteStatement substituteStatement = NULL;
            substituteStatement = parseSubstituteStatement(parser);
            if (substituteStatement != NULL)
            {
                Logger_dbg("Substitute statement is created.");
                statement = statement_new(STATEMENT_KIND_SUBSTITUTE);
                statement->of._substitute_ = substituteStatement;
                statement->line = line;
            }
        }
        else
        {
            Logger_dbg("Function call statement parse.");
            FunctionCallStatement functionCallStatement = NULL;
            functionCallStatement = parseFunctionCallStatement(parser);
            if (functionCallStatement != NULL)
            {
                Logger_dbg("Function call statement is created.");
                statement = statement_new(STATEMENT_KIND_FUNCTION_CALL);
                statement->of._functionCall_ = functionCallStatement;
                statement->line = line;
            }
        }
        break;
    default:
        //構文エラーじゃね？
        if (parser_getNext(parser) != NULL)
        {
            parser_error(current);
        }
        break;
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
END:
    */
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}












































