#include "_Cria.h"



Reference
reference_new(
    ReferenceType   type
)
{
    Reference reference = NULL;
    
    reference = Memory_malloc(sizeof(struct ReferenceTag));
    memset(reference, 0x00, sizeof(struct ReferenceTag));
    reference->type = type;
    
    return reference;
}



ReferenceVariable
reference_variable(
    Parser  parser,
    String  name
)
{
    Logger_trc("[ START ]%s", __func__);
    ReferenceVariable reference = NULL;
    
    reference = Memory_malloc(sizeof(struct ReferenceVariableTag));
    memset(reference, 0x00, sizeof(struct ReferenceVariableTag));
    reference->name = name;
    
    parser_next(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



ReferenceClass
reference_class(
    Parser  parser,
    String  name
)
{
    Logger_trc("[ START ]%s", __func__);
    ReferenceClass reference = NULL;
    
    reference = Memory_malloc(sizeof(struct ReferenceClassTag));
    memset(reference, 0x00, sizeof(struct ReferenceClassTag));
    reference->name = name;
    
    parser_next(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



ReferenceFunctionCall
reference_functionCall(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = parser_getCurrent(parser);
    ReferenceFunctionCall reference = NULL;
    String functionName = NULL;
    ParametersExpression parametersExpression = NULL;
    
    
    //最初のトークンを評価し、識別子でなければNULL返却。
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("First token is not identifier.");
        goto END;
    }
    
    //OKなら識別子を関数名として取得
    functionName = string_clone(token->buffer);
    
    //次のトークンがなければNULLを返却
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Second token does not exist.");
        goto END;
    }
    
    //あればそれを取得
    token = parser_getCurrent(parser);
    
    //左括弧でなければFALSE返却
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        goto END;
    }
    
    //次のトークンがなければFALSEを返却
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Third token does not exist.");
        goto END;
    }
    
    //引数をパース
    parametersExpression = expression_parseParametersExpression(parser);
    if (parametersExpression == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        goto END;
    }
    
    //現在のトークンを取得
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    
    //右括弧でなければFALSE返却
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    //次のトークンへ移動しておく。
    parser_next(parser);
    
    
    reference = Memory_malloc(sizeof(struct ReferenceFunctionCallTag));
    memset(reference, 0x00, sizeof(struct ReferenceFunctionCallTag));
    reference->name = functionName;
    reference->parameters = parametersExpression;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
reference_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Reference reference = NULL;
    ReferenceVariable variable = NULL;
    ReferenceFunctionCall function = NULL;
    ReferenceClass klass = NULL;
    
    Token token = NULL;
    Token next = NULL;
    
    
    //現在のトークンと次のトークンへの参照を取得
    Logger_dbg("Get current token.");
    token = parser_getCurrent(parser);
    token_log(token);
    if (token == NULL)
    {
        Logger_dbg("Token is NULL.");
        goto END;
    }
    
    
    next = parser_getNext(parser);
    
    
    //ピリオドの場合（自己インスタンス参照）
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Logger_dbg("Token is period.");
        //次のトークンからの解析
        if (parser_next(parser) == FALSE)
        {
            parser_error(next);
            goto END;
        }
        
        //先頭のトークンがピリオドだった場合は自己インスタンス、またはクラスへの参照とみなす。
        Logger_dbg("Create self reference.");
        reference = reference_new(REFERENCE_TYPE_SELF);
        reference->of.variable = NULL;
        goto NEXT_REFERENCE;
    }
    
    
    //識別子の場合
    if (token->type == TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Token is Identifier.");
        //次のトークンがピリオドならオブジェクト参照、左括弧ならメソッド呼び出し、
        if (next->type == TOKEN_TYPE_PARENTHESIS_LEFT)
        {
            Logger_dbg("Create reference function call.");
            function = reference_functionCall(parser);
            
            reference = reference_new(REFERENCE_TYPE_FUNCTION_CALL);
            reference->of.function = function;
            
            token_log(parser_getCurrent(parser));
            
            goto NEXT_REFERENCE;
        }
        else
        {
            Logger_dbg("Create variable reference.");
            variable = reference_variable(parser, token->buffer);
            
            reference = reference_new(REFERENCE_TYPE_VARIABLE);
            reference->of.variable = variable;
            
            parser_next(parser);
            
            if (next->type == TOKEN_TYPE_PERIOD)
            {
                goto NEXT_REFERENCE;
            }
            else if (next->type == TOKEN_TYPE_COLON)
            {
                goto END;
            }
            else
            {
                parser_error(next);
            }
        }
    }
    else if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("Token is Class literal.");
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            Logger_dbg("Create class reference.");
            klass = reference_class(parser, token->buffer);
            
            reference = reference_new(REFERENCE_TYPE_CLASS);
            reference->of.klass = klass;
            
            goto NEXT_REFERENCE;
        }
        else
        {
            Logger_dbg("No match reference expression.");
            parser_error(next);
            goto END;
        }
    }
    
    //上記いずれのパターンにも当てはまらない場合はNULLを返却
    goto END;
    
NEXT_REFERENCE:
    //次の参照を確認
    Logger_dbg("Create next reference expression.");
    reference->next = reference_parse(parser);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
reference_getLast(
    Reference reference
)
{
    if (reference->next == NULL)
        return reference;
    
    return reference_getLast(reference->next);
}




