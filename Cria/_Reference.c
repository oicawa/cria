#include "_Cria.h"



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
        reference = Memory_malloc(sizeof(struct ReferenceTag));
        memset(reference, 0x00, sizeof(struct ReferenceTag));
        reference->type = REFERENCE_TYPE_SELF;
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
            
            reference = Memory_malloc(sizeof(struct ReferenceTag));
            memset(reference, 0x00, sizeof(struct ReferenceTag));
            reference->type = REFERENCE_TYPE_FUNCTION_CALL;
            reference->of.function = function;
            
            token_log(parser_getCurrent(parser));
            
            goto NEXT_REFERENCE;
        }
        else
        {
            Logger_dbg("Create variable reference.");
            variable = reference_variable(parser, token->buffer);
            
            reference = Memory_malloc(sizeof(struct ReferenceTag));
            memset(reference, 0x00, sizeof(struct ReferenceTag));
            reference->type = REFERENCE_TYPE_VARIABLE;
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
        }
    }
    else if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("Token is Class literal.");
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            Logger_dbg("Create class reference.");
            klass = reference_class(parser, token->buffer);
            
            reference = Memory_malloc(sizeof(struct ReferenceTag));
            memset(reference, 0x00, sizeof(struct ReferenceTag));
            reference->type = REFERENCE_TYPE_CLASS;
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




