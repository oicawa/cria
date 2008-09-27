#include <stdio.h>

#include "_Cria.h"


//========================================
//CommentStatement
//========================================
ReturnStatement
returnStatement_new(
    void
)
{
    ReturnStatement statement = Memory_malloc(sizeof(struct ReturnStatementTag));
    statement->expression = NULL;
    return statement;
}



void
returnStatement_dispose(
    ReturnStatement statement
)
{
    Logger_trc("[ START ]%s", __func__);
    if (statement == NULL)
    {
        goto END;
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



ReturnStatement
returnStatement_parse(
    char*       line,
    int         offset
)
{
    ReturnStatement statement = NULL;
    Expression      expression = NULL;
    
    
    //後置変数なしのreturn
    if (strncmp(&line[offset], RETURN_LITERAL_1, strlen(RETURN_LITERAL_1)) == 0)
    {
        Logger_trc("'return' statement created.");
        return returnStatement_new();
    }
    
    
    if (strncmp(&line[offset], RETURN_LITERAL_2, strlen(RETURN_LITERAL_2)) != 0)
    {
        Logger_trc("Not 'return' statement.");
        return NULL;
    }
    
    expression = expression_parse(line, offset + strlen(RETURN_LITERAL_2));
    if (expression == NULL)
    {
        Logger_trc("Expression for 'return' parse error.");
        return NULL;
    }
    
    statement = returnStatement_new();
    statement->expression = expression;
    Logger_trc("'return EXPRESSION' statement created.");
    return statement;
}



