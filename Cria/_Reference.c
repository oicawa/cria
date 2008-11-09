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
    Reference reference = NULL;
    ReferenceVariable variable = NULL;
    Item position = parser_getPosition(parser); //バックアップ用
    String name = null;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        parser_setPosition(position);
        goto END:
    }
    
    //変数名を保持
    name = token->buffer;
    
    //次のトークンがピリオド、または「 = 」以外ならば対象外なので終了。
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD && token->type != TOKEN_TYPE_SUBSTITUTE)
    {
        parser_setPosition(position);
        goto END:
    }
    
    //該当した場合はオブジェクトを生成
    variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    memset(variable, 0x00, sizeof(struct ReferenceVariableTag));
    variable->name = String_clone(name);

    //Referenceを生成
    reference = reference_new(REFERENCE_TYPE_VARIABLE);
    reference->of.variable = variable;
    
    //代入トークンだった場合はそこで戻る。
    if (token->type == TOKEN_TYPE_SUBSTITUTE)
        goto END;
    
    //以下、ピリオドだった場合。
    //トークンを進めて改めてreferenceでパースし何も帰ってこなければエラー
    parser_next(parser);
    reference->next = reference_parse(parser);
    if (reference->next == null)
        parser_error(token);
    
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
    reference = reference_variable(parser);
    if (reference != null)
        goto END;
    
    //★保留中
    //reference = reference_functionCall(parser);
    //if (reference != null)
    //    goto END;
    
    //★保留中
    //reference = reference_class(parser);
    //if (reference != null)
    //    goto END;
        
    //いずれでもなかった場合はパーサー位置を戻す。
    parser_setPosition(position);
    
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




