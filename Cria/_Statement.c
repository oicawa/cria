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
statement_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    
    
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
    
    //代入式
    Logger_dbg("Check 'SubstituteStatement'");
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
    FunctionCallStatement functionCallStatement = NULL;
    functionCallStatement = functionCallStatement_parse(parser);
    if (functionCallStatement != NULL)
    {
        Logger_dbg("Create 'FunctionCallStatement'");
        //statement = statement_new(EXPRESSION_KIND_FUNCTION_CALL);
        statement = statement_new(STATEMENT_KIND_FUNCTION_CALL);
        statement->of._functionCall_ = functionCallStatement;
        goto END;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}












































