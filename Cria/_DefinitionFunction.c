#include "_Cria.h"

FunctionDefinition
functionDefinition_new(
    char*               name,
    Boolean             isNative,
    AccessLevel         access,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
)
{
    FunctionDefinition definition = Memory_malloc(sizeof(struct FunctionDefinitionTag));
    memset(definition, 0x00, sizeof(struct FunctionDefinitionTag));
    definition->name = name;
    definition->isNative = isNative;
    definition->access = access;
    if (isNative == TRUE)
    {
        definition->of.native.function = nativeFunctionPoint;
    }
    else
    {
        definition->of.cria.parameterList = parameterList;
        definition->of.cria.statementList = statementList;
    }

    return definition;
}



Boolean
functionDefinition_isMatch(
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
    
    //先頭のトークンが識別子でなければFALSE。
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        goto END;
    }
    
    //次のトークンへ。
    if (parser_next(parser) == FALSE)
    {
        goto END;
    }
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        goto END;
    }
    
    //改行トークンが検知されるまでに、コロントークンを検知したらOK。
    while (parser_next(parser) == TRUE)
    {
        //改行トークン？
        token = parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_NEW_LINE)
        {
            goto END;
        }
        if (token->type == TOKEN_TYPE_COLON)
        {
            result = TRUE;
            break;
        }
    }
    
END:
    parser_returnToMark(parser, mark);
    return result;
}



//本当はHashを使いたいが、とにかく動くものを先に作りたいので、
//線形検索を行う。遅いけど。
FunctionDefinition
functionDefinition_search(
    List    functions,
    char*   name
)
{
    int count = functions->count;
    int index = 0;
    FunctionDefinition definition = NULL;
    FunctionDefinition tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (FunctionDefinition)(list_get(functions, index));
        if (strcmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}




