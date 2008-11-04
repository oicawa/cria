#include "_Cria.h"



VariableDefinition
variableDefinition_new(
    String      name,
    String      type,
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



void
variableDefinition_parse(
    List    variableList,
    Parser  parser
)
{
    Token token = NULL;
    VariableDefinition definition = NULL;
    String      name = NULL;
    String      type = string_new("Object");
    
    //先頭のトークンが識別子か定数リテラルでなければFALSE。
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER || token->type != TOKEN_TYPE_CONSTANT)
    {
        parser_error(token);
    }
    name = token->buffer;
    
    //次のトークンが、コロンでなければFALSE。
    if (parser_next(parser) == FALSE)
    {
        parser_error(token);
    }
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_COLON)
    {
        parser_error(token);
    }
    
    
    //次のトークンが改行トークンだった場合はオブジェクト型＆NULLで初期化。
    if (parser_next(parser) == FALSE)
    {
        parser_error(token);
    }
    
    token = parser_getCurrent(parser);
    
    
    
    //型チェック
    if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        type = string_clone(token->buffer);
        if (parser_next(parser) == FALSE)
        {
            parser_error(token);
        }
    }
    
    //改行トークン以外？
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        parser_error(token);
    }
    
    definition = Memory_malloc(sizeof(struct VariableDefinitionTag));
    memset(definition, 0x00, sizeof(struct VariableDefinitionTag));
    definition->name = name;
    definition->type = type;
    
    
    //定義を登録
    list_add(variableList, definition);
    
    
    return;
}



VariableDefinition
definition_search(
    List    list,
    String  name
)
{
    int count = localList->count;
    int index = 0;
    VariableDefinition definition = NULL;
    VariableDefinition tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (VariableDefinition)(list_get(variableList, index));
        if (strcmp(tmp->name->pointer, name->pointer) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}


VariableDefinition
variableDefinition_search(
    List        globalList,
    List        fieldList,
    List        localList,
    Reference   reference
)
{
    int count = localList->count;
    int index = 0;
    VariableDefinition definition = NULL;
    VariableDefinition tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (VariableDefinition)(list_get(variableList, index));
        if (strcmp(tmp->name->pointer, name->pointer) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



