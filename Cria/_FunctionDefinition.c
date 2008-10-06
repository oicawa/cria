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
    
    //NULL�`�F�b�N
    if (parser == NULL)
    {
        goto END;
    }
    
    //���݂̃g�[�N���𕜋A�ʒu�Ƃ��ăo�b�N�A�b�v
    mark = parser->current;
    
    //�擪�̃g�[�N�������ʎq�łȂ����FALSE�B
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        goto END;
    }
    
    //���̃g�[�N���ցB
    if (parser_next(parser) == FALSE)
    {
        goto END;
    }
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        goto END;
    }
    
    //���s�g�[�N�������m�����܂łɁA�R�����g�[�N�������m������OK�B
    while (parser_next(parser) == TRUE)
    {
        //���s�g�[�N���H
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

