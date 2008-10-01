#include <stdio.h>

#include "_Cria.h"



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



//OperationExpression�j��
void
operationExpression_dispose(
    OperationExpression expression
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



OperationExpression
operationExpression_parse(
    Parser  parser
)
{
    OperationExpression expression = NULL;
    
    /*
    Token token = parser_getCurrent(parser);
    
    
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("First token is not identifier.");
        goto END;
    }
    
    //OK�Ȃ环�ʎq���֐����Ƃ��Ď擾
    functionName = string_clone(token->buffer);
    
    //���̃g�[�N�����Ȃ����NULL��ԋp
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Second token does not exist.");
        goto END;
    }
    
    //����΂�����擾
    Logger_dbg("Get second token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got second token.");
    
    //�����ʂłȂ����FALSE�ԋp
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        goto END;
    }
    
    //���̃g�[�N�����Ȃ����FALSE��ԋp
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Third token does not exist.");
        goto END;
    }
    
    //�������p�[�X
    parametersExpression = parametersExpression_parse(parser);
    if (parametersExpression == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        goto END;
    }
    
    //���݂̃g�[�N�����擾
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    
    //�E���ʂłȂ����FALSE�ԋp
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    expression = functionCallExpression_new(functionName, parametersExpression);
    
    
    
    
    /*
    switch (parser->next)
    {
    case TOKEN_TYPE_PLUS:
        break;
    case TOKEN_TYPE_MINUS:
        break;
    case TOKEN_TYPE_MULTIPLY:
        break;
    case TOKEN_TYPE_DEVIDE:
        break;
    case TOKEN_TYPE_MODULO:
        break;
    case TOKEN_TYPE_PARENTHESIS_LEFT:
        break;
    case TOKEN_TYPE_PARENTHESIS_RIGHT:
        break;
    case TOKEN_TYPE_LESS_THAN:
        break;
    case TOKEN_TYPE_LESS_EQUAL:
        break;
    case TOKEN_TYPE_EQUAL:
        break;
    case TOKEN_TYPE_NOT_EQUAL:
        break;
    case TOKEN_TYPE_OR:
        break;
    case TOKEN_TYPE_AND:
        break;
    case TOKEN_TYPE_GREATER_EQUAL:
        break;
    case TOKEN_TYPE_GREATER_THAN:
        break;
    default:
        break;
    }
    

OPERATION_KIND_PLUS
OPERATION_KIND_MINUS
OPERATION_KIND_MULTIPLY
OPERATION_KIND_DIVIDE
OPERATION_KIND_MODULO
OPERATION_KIND_EQUAL
OPERATION_KIND_LESS_THAN
OPERATION_KIND_LESS_EQUAL
    //*/
    
    
    
    return expression;
}







