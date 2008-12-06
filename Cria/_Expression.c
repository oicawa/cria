#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_Expression.h"


Expression
expression_new(
    ExpressionKind  kind
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = Memory_malloc(sizeof(struct ExpressionTag));
    memset(expression, 0x00, sizeof(struct ExpressionTag));
    expression->kind = kind;
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



