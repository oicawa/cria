#include <stdio.h>

#include "_Cria.h"



VariableExpression
variableExpression_new(
    String                  name
)
{
    VariableExpression expression = Memory_malloc(sizeof(struct VariableExpressionTag));
    memset(expression, 0x00, sizeof(struct VariableExpressionTag));
    expression->name = name;
    return expression;
}



void
variableExpression_dispose(
    VariableExpression   expression
)
{
    Logger_trc("[ START ]%s", __func__);
    if (expression == NULL)
    {
        goto END;
    }
    
    
    if (expression->name != NULL)
    {
        string_dispose(expression->name);
        expression->name = NULL;
    }
    
    Memory_free(expression);
    expression = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



VariableExpression
variableExpression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = parser_getCurrent(parser);
    VariableExpression expression = NULL;
    String variableName = NULL;
    
    
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("First token is not identifier.");
        goto END;
    }
    
    variableName = string_clone(token->buffer);
    
    expression = variableExpression_new(variableName);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}















































