#include "_Cria.h"



StringLiteralExpression
stringLiteralExpression_new(
    String  value
)
{
    StringLiteralExpression expression = Memory_malloc(sizeof(struct StringLiteralExpressionTag));
    memset(expression, 0x00, sizeof(struct StringLiteralExpressionTag));
    expression->value = string_clone(value);
    return expression;
}



//Expression”jŠü
void
stringLiteralExpression_dispose(
    StringLiteralExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    if (expression == NULL)
    {
        goto END;
    }
    
    if (expression->value != NULL)
    {
        string_dispose(expression->value);
        expression->value = NULL;
    }
        
    Memory_free(expression);
    expression = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



StringLiteralExpression
stringLiteralExpression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    StringLiteralExpression stringLiteralExpression = NULL;
    Token token = parser_getCurrent(parser);
    String string = NULL;
    
    token_log(token);
    Logger_dbg("Compare token type is %d", TOKEN_TYPE_STRING_LITERAL);
    
    if (token->type != TOKEN_TYPE_STRING_LITERAL)
    {
        Logger_dbg("This is not a string literal token.");
        goto END;
    }
    
    
    Logger_dbg("Create string literal.");
    string = string_clone(token->buffer);
    stringLiteralExpression = stringLiteralExpression_new(string);
    
END:
    
    Logger_trc("[  END  ]%s", __func__);
    return stringLiteralExpression;
}







