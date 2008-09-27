#include <stdio.h>

#include "_Cria.h"


//========================================
//CommentStatement
//========================================
WhileStatement
whileStatement_new(
    void
)
{
    WhileStatement statement = Memory_malloc(sizeof(struct WhileStatementTag));
    statement->statements = NULL;
    return statement;
}



void
whileStatement_dispose(
    WhileStatement  statement
)
{
    Logger_trc("[ START ]%s", __func__);
    if (statement == NULL)
    {
        goto END;
    }
    
    if (statement->condition != NULL)
    {
        Memory_free(statement->condition);
        statement->condition = NULL;
    }
    Memory_free(statement);
    statement = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



WhileStatement
whileStatement_parse(
    Interpreter interpreter,
    char*       buffer,
    int         offset
)
{
    WhileStatement  statement = NULL;
    Expression      condition = NULL;
    
    
    if (strncmp(&buffer[offset], WHILE_LITERAL, strlen(WHILE_LITERAL)) != 0)
    {
        Logger_trc("Not 'while' statement.");
        return NULL;
    }
    
    
    condition = expression_parse(buffer, offset + strlen(WHILE_LITERAL));
    if (condition == NULL)
    {
        Logger_trc("Expression for 'return' parse error.");
        return NULL;
    }
    
    statement = whileStatement_new();
    statement->condition = condition;
    Logger_trc("'while' statement created.");
    return statement;
}



