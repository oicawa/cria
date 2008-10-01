#include <stdio.h>

#include "_Cria.h"



SubstituteStatement
substituteStatement_new(
    VariableExpression  variableExpression,
    Expression          expression
)
{
    Logger_trc("[ START ]%s", __func__);
    SubstituteStatement statement = NULL;
    statement = Memory_malloc(sizeof(struct SubstituteStatementTag));
    memset(statement, 0x00, sizeof(struct SubstituteStatementTag));
    statement->left = variableExpression;
    statement->right = expression;
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
    
    if (statement->left != NULL)
    {
        variableExpression_dispose(statement->left);
        statement->left = NULL;
    }
    
    if (statement->right != NULL)
    {
        expression_dispose(statement->right);
        statement->right = NULL;
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
    VariableExpression variableExpression = NULL;
    Expression expression = NULL;
    Token token = NULL;
    
    
    Logger_dbg("Check 'VariableExpression'");
    variableExpression = variableExpression_parse(parser);
    if (variableExpression == NULL)
    {
        Logger_dbg("Not variable expression.");
        goto END;
    }
    
    
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
        Logger_dbg("It is not new line token.");
        goto END;
    }
    
    
    Logger_dbg("Check 'Expression'");
    expression = expression_parse(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not expression.");
        goto END;
    }
    
    
    Logger_dbg("Create 'SubstituteStatement'");
    substituteStatement = substituteStatement_new(variableExpression, expression);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return substituteStatement;
}
    

