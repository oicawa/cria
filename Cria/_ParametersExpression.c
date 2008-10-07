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
    
    //右括弧のトークンが出現するまで繰り返し
    token = parser_getCurrent(parser);
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        //式解析。
        expression = expression_parse(parser);
        if (expression == NULL)
        {
            Logger_dbg("Not expression.");
            goto END;
        }
        
        //解析できたらパラメータとして追加
        list_add(list, expression);
        
        //次のトークンへ
        if (parser_next(parser) == FALSE)
        {
            Logger_err("Next token is nothing.");
            perror("Next token is nothing.");
            goto END;
        }
        
        //トークンが', 'であれば更に読み出しを行って継続
        token = parser_getCurrent(parser);
        token_log(token);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            //次のトークンへ
            if (parser_next(parser) == FALSE)
            {
                Logger_err("Next token is nothing.");
                perror("Next token is nothing.");
                goto END;
            }
            continue;
        }
        
        //トークンが', 'でも右括弧でもない場合はエラー
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












































