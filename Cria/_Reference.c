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



Reference
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
    Item position = parser_getPosition(parser);
    Reference reference = NULL;
    
    //現在のトークンと次のトークンへの参照を取得
    Logger_dbg("Get current token.");
    reference = reference_variable(parser);
    if (reference != null)
        goto PARSE_NEXT;
    
    reference = reference_function(parser);
    if (reference != null)
        goto PARSE_NEXT;
    
    reference = reference_class(parser);
    if (reference != NULL)
        goto PARSE_NEXT;
    
    //いずれのパースでも処理できなかった場合はNULLを返却
    goto END;

PARSE_NEXT:
    //次の参照を生成
    next = reference_parser(parser);
    reference->next = next;

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




