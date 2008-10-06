#include "_Cria.h"



VariableDefinition
variableDefinition_new(
    char*       name,
    char*       type,
    AccessLevel access,
    CriaId      object
)
{
    VariableDefinition definition = Memory_malloc(sizeof(struct VariableDefinitionTag));
    memset(definition, 0x00, sizeof(struct VariableDefinitionTag));
    definition->name = name;
    definition->type = type;
    definition->access = access;
    definition->object = object;

    return definition;
}



Boolean
variableDefinition_isMatch(
    Parser parser
)
{
    Boolean result = FALSE;
    Item mark = NULL;
    Token token = NULL;
    
    //NULLチェック
    if (parser == NULL)
    {
        goto END;
    }
    
    //現在のトークンを復帰位置としてバックアップ
    mark = parser->current;
    
    //先頭のトークンが識別子か定数リテラルでなければFALSE。
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER || token->type != TOKEN_TYPE_CONSTANT)
    {
        goto END;
    }
    
    //次のトークンが、コロンでなければFALSE。
    if (parser_next(parser) == FALSE)
    {
        goto END;
    }
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_COLON)
    {
        goto END;
    }
    
    //変数宣言扱い。
    result = TRUE;
    
END:
    parser_returnToMark(parser, mark);
    return result;
}

