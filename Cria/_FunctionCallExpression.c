#include <stdio.h>

#include "_Cria.h"



FunctionCallExpression
functionCallExpression_new(
    String                  name,
    ParametersExpression    parameters
)
{
    FunctionCallExpression expression = Memory_malloc(sizeof(struct FunctionCallExpressionTag));
    memset(expression, 0x00, sizeof(struct FunctionCallExpressionTag));
    expression->name = name;
    expression->parameters = parameters;
    return expression;
}



void
functionCallExpression_dispose(
    FunctionCallExpression   expression
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
    
    if (expression->parameters != NULL)
    {
        parametersExpression_dispose(expression->parameters);
        expression->parameters = NULL;
    }
    
    Memory_free(expression);
    expression = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



FunctionCallExpression
functionCallExpression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = parser_getCurrent(parser);
    FunctionCallExpression expression = NULL;
    String functionName = NULL;
    ParametersExpression parametersExpression = NULL;
    
    
    //�ŏ��̃g�[�N����]�����A���ʎq�łȂ����NULL�ԋp�B
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
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}















































