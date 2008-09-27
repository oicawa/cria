#include <stdio.h>

#include "_Cria.h"



ExpressionStatement
expressionStatement_new(
    ExpressionKind  kind
)
{
    ExpressionStatement expression = Memory_malloc(sizeof(struct ExpressionStatementTag));
    memset(expression, 0x00, sizeof(struct ExpressionStatementTag));
    expression->kind = kind;
    return expression;
}



void
expressionStatement_dispose(
    ExpressionStatement  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    
    if (expression == NULL)
    {
        goto END:
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



//式文になりうるのは、「代入式」と「関数呼び出し式」のみ構成できる。
ExpressionStatement
expressionStatement_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionStatement  expressionStatement = NULL;
    
    
    //関数呼び出し式
    Logger_dbg("Check 'FunctionCallExpression'");
    FunctionCallExpression functionCallExpression = NULL;
    functionCallExpression = functionCallExpression_parse(parser);
    if (functionCallExpression != NULL)
    {
        Logger_dbg("Create 'FunctionCallExpression'");
        expression = expressionStatement_new(EXPRESSION_KIND_FUNCTION_CALL);
        expression->of._functionCall_ = functionCallExpression;
        goto NEW_LINE_CHEK;
    }
    
NEW_LINE_CHEK:
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}












































