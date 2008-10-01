#include <stdio.h>

#include "_Cria.h"



Expression
expression_new(
    ExpressionKind  kind
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = Memory_malloc(sizeof(struct ExpressionTag));
    memset(expression, 0x00, sizeof(struct ExpressionTag));
    expression->kind = kind;
    Logger_trc("expression kind is '%d'", expression->kind);
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



void
expression_dispose(
    Expression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    if (expression == NULL)
    {
        goto END;
    }
    
    switch (expression->kind)
    {
    case EXPRESSION_KIND_OPERATION:
        //operationExpression_dispose(expression->of._operation_);
        break;
    case EXPRESSION_KIND_STRING_LITERAL:
        stringLiteralExpression_dispose(expression->of._stringLiteral_);
        expression->of._stringLiteral_ = NULL;
        break;
    case EXPRESSION_KIND_INTEGER_LITERAL:
        break;
    case EXPRESSION_KIND_REAL_LITERAL:
        break;
    case EXPRESSION_KIND_BOOLEAN_LITERAL:
        break;
    case EXPRESSION_KIND_NULL_LITERAL:
        break;
    case EXPRESSION_KIND_GENERATE:
        break;
    case EXPRESSION_KIND_FUNCTION_CALL:
        //functionCallExpression_dispose(expression->of._functionCall_);
        break;
    case EXPRESSION_KIND_VARIABLE:
        break;
    default:
        Logger_err("Illegal statement type. (%d)", expression->kind);
        break;
    }
    Memory_free(expression);
    expression = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



Expression
expression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    
    while (1)
    {
        Logger_dbg("Check 'OperationExpression'");
        OperationExpression operationExpression = NULL;
        operationExpression = operationExpression_parse(parser);
        if (operationExpression != NULL)
        {
            Logger_dbg("Create 'StringLiteralExpression'");
            expression = expression_new(EXPRESSION_KIND_OPERATION);
            expression->of._operation_ = operationExpression;
            goto END;
        }
    
        Logger_dbg("Check 'StringLiteralExpression'");
        StringLiteralExpression stringLiteralExpression = NULL;
        stringLiteralExpression = stringLiteralExpression_parse(parser);
        if (stringLiteralExpression != NULL)
        {
            Logger_dbg("Create 'StringLiteralExpression'");
            expression = expression_new(EXPRESSION_KIND_STRING_LITERAL);
            expression->of._stringLiteral_ = stringLiteralExpression;
            goto END;
        }
        
        
        Logger_dbg("Check 'FunctionCallExpression'");
        FunctionCallExpression functionCallExpression = NULL;
        functionCallExpression = functionCallExpression_parse(parser);
        if (functionCallExpression != NULL)
        {
            Logger_dbg("Create 'FunctionCallExpression'");
            expression = expression_new(EXPRESSION_KIND_FUNCTION_CALL);
            expression->of._functionCall_ = functionCallExpression;
            goto END;
        }
        
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}












































