#include <stdio.h>

#include "_Cria.h"



ParametersExpression
parametersExpression_new(
    List    list
)
{
    ParametersExpression expression = NULL;
    expression = Memory_malloc(sizeof(struct ParametersExpressionTag));
    memset(expression, 0x00, sizeof(struct ParametersExpressionTag));
    expression->list = list;
    return expression;
}



void
parametersExpression_dispose(
    ParametersExpression    expression
)
{
    Logger_trc("[ START ]%s", __func__);
    if (expression == NULL)
        goto END;
    
    if (expression->list != NULL)
    {
        list_dispose(expression->list);
        expression->list = NULL;
    }
    
    Memory_free(expression);
    expression = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



ParametersExpression
parametersExpression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ParametersExpression parameters = NULL;
    Expression expression = NULL;
    Token token = NULL;
    List list = list_new();
    
    //�E���ʂ̃g�[�N�����o������܂ŌJ��Ԃ�
    token = parser_getCurrent(parser);
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        //����́B
        expression = expression_parse(parser);
        if (expression == NULL)
        {
            Logger_dbg("Not expression.");
            goto END;
        }
        
        //��͂ł�����p�����[�^�Ƃ��Ēǉ�
        list_add(list, expression);
        
        //���̃g�[�N����
        if (parser_next(parser) == FALSE)
        {
            Logger_err("Next token is nothing.");
            perror("Next token is nothing.");
            goto END;
        }
        
        //�g�[�N����', '�ł���΍X�ɓǂݏo�����s���Čp��
        token = parser_getCurrent(parser);
        token_log(token);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            //���̃g�[�N����
            if (parser_next(parser) == FALSE)
            {
                Logger_err("Next token is nothing.");
                perror("Next token is nothing.");
                goto END;
            }
            continue;
        }
        
        //�g�[�N����', '�ł��E���ʂł��Ȃ��ꍇ�̓G���[
        if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            Logger_err("Next token is illegal.");
            perror("Next token is illegal.");
            goto END;
        }
    }
    
    parameters = parametersExpression_new(list);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return parameters;
}












































