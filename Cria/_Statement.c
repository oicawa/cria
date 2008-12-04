#include <stdio.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_Statement.h"



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
    case STATEMENT_KIND_SUBSTITUTE:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        //ifStatement_dispose(statement->of._if_);
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
    case STATEMENT_KIND_GOTO:
        //returnStatement_dispose(statement->of._return_);
        break;
    case STATEMENT_KIND_CATCH:
        //finallyStatement_dispose(statement->of._catch_);
        break;
    case STATEMENT_KIND_FINALLY:
        //finallyStatement_dispose(statement->of._finally_);
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



