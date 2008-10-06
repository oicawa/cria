#include "_Cria.h"

ClassDefinition
classDefinition_new(
    char*   name,
    List    baseList,
    List    fieldList,
    List    methodList
)
{
    ClassDefinition definition = Memory_malloc(sizeof(struct ClassDefinitionTag));
    memset(definition, 0x00, sizeof(struct ClassDefinitionTag));
    definition->name = name;
    definition->baseList = baseList;
    definition->fieldList = fieldList;
    definition->methodList = methodList;

    return definition;
}



Boolean
classDefinition_isMatch(
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
    
    //先頭のトークンがクラスリテラルでなければFALSE。
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
        goto END;
    }
    
    //次のトークンへ。
    if (parser_next(parser) == FALSE)
    {
        goto END;
    }
    token = parser_getCurrent(parser);
    
    //次のトークンが改行トークンか、継承トークンの場合はOK
    if (token->type == TOKEN_TYPE_NEW_LINE || token->type == TOKEN_TYPE_EXTENDS)
    {
        result = TRUE;
        goto END;
    }
    
    
END:
    parser_returnToMark(parser, mark);
    return result;
}

