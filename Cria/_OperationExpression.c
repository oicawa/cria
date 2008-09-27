#include <stdio.h>

#include "_Cria.h"



//OperationExpressionê∂ê¨
OperationExpression
operationExpression_new(
    OperationExpressionKind  kind
)
{
    OperationExpression expression = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(expression, 0x00, sizeof(struct OperationExpressionTag));
    expression->kind = kind;
    return expression;
}



//OperationExpressionîjä¸
void
operationExpression_dispose(
    OperationExpress    expression
)
{
    Logger_trc("[ START ]%s", __func__);
    if (exrepssion == NULL)
        goto END;
    
    if (expression->left != NULL)
    {
        Expression_dispose(expression->left);
        expression->left = NULL;
    }
    
    if (expression->right != NULL)
    {
        Expression_dispose(expression->right);
        expression->right = NULL;
    }
    
    Memory_free(expression);
    expression = NULL;
END:
    Logger_trc("[  END  ]%s", __func__);
}



//OperationExpressionÉpÅ[ÉX
OperationExpression
operationExpression_parse(
    char*       buffer,
    int         offset
)
{
    OperationExpression expression = NULL;
    
    
    return NULL;
}







