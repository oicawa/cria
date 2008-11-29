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
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Reference reference = NULL;
    ReferenceVariable variable = NULL;
    Item position = parser_getPosition(parser); //バックアップ用
    String name = NULL;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
    	token_log(token);
    	Logger_dbg("Not identifier.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->buffer;
	Logger_dbg("Variable name is '%s'", name->pointer);
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD && token->type != TOKEN_TYPE_SUBSTITUTE)
    {
    	token_log(token);
    	Logger_dbg("Not '.' and ' = '");
        parser_setPosition(parser, position);
        goto END;
    }
    
    //該当した場合はオブジェクトを生成
    variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    memset(variable, 0x00, sizeof(struct ReferenceVariableTag));
    variable->name = string_clone(name);
	Logger_dbg("Created ReferenceVariable");

    //Referenceを生成
    reference = reference_new(REFERENCE_TYPE_VARIABLE);
    reference->of.variable = variable;
    
    //代入トークンだった場合はそこで戻る。
    if (token->type != TOKEN_TYPE_PERIOD)
    {
    	token_log(token);
    	Logger_dbg("Not '.'");
        goto END;
    }
    
    
    //以下、ピリオドだった場合。
    //トークンを進めて改めてreferenceでパースし何も帰ってこなければエラー
    parser_next(parser);
    token = parser_getCurrent(parser);
    token_log(token);
    reference->next = reference_parse(parser);
    if (reference->next == NULL)
        parser_error(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
reference_functionCall(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    Reference reference = NULL;
    ReferenceFunctionCall functionCall = NULL;
    ParametersExpression parameters = NULL;
    String name = NULL;
    
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
    	token_log(token);
	    Logger_dbg("Not identifier.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->buffer;
    Logger_dbg("function name is '%s'", name->pointer);
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
    	token_log(token);
	    Logger_dbg("Not '('.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    parser_next(parser);
    parameters = expression_parseParametersExpression(parser);
    if (parameters == NULL)
    {
	    Logger_dbg("Not parameters.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
    	token_log(token);
	    Logger_dbg("Not ')'.");
    	parser_error(token);
        goto END;
    }
    
    functionCall = Memory_malloc(sizeof(struct ReferenceFunctionCallTag));
    memset(functionCall, 0x00, sizeof(struct ReferenceFunctionCallTag));
    functionCall->name = string_clone(name);
    functionCall->parameters = parameters;
    Logger_dbg("Created ReferenceFunctionCall");

    reference = reference_new(REFERENCE_TYPE_FUNCTION_CALL);
    reference->of.function = functionCall;
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD)
    {
    	token_log(token);
	    Logger_dbg("Not '.'.");
    	goto END;
    }
    
	parser_next(parser);
	reference->next = reference_parse(parser);
	if (reference->next == NULL)
	{
	    Logger_dbg("Not exist next Reference after '.'");
		parser_error(token);
	}
	
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
    if (reference != NULL)
        goto END;
    
    reference = reference_functionCall(parser);
    if (reference != NULL)
        goto END;
    
    //★保留中
    //reference = reference_class(parser);
    //if (reference != null)
    //    goto END;
        
    //いずれでもなかった場合はパーサー位置を戻す。
    parser_setPosition(parser, position);
    
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




