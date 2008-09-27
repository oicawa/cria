#include <stdio.h>

#include "_Cria.h"


//========================================
//IfStatement
//========================================
IfStatement
ifStatement_new(
    void
)
{
    IfStatement statement = Memory_malloc(sizeof(struct IfStatementTag));
    statement->condition = NULL;
    statement->statements = List_new();
    statement->_if_ = NULL;
    return statement;
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
    
    expression = expression_parse(interpreter, line, offset + strlen(RETURN_LITERAL_2));
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



