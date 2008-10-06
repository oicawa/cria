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
    
    //NULL�`�F�b�N
    if (parser == NULL)
    {
        goto END;
    }
    
    //���݂̃g�[�N���𕜋A�ʒu�Ƃ��ăo�b�N�A�b�v
    mark = parser->current;
    
    //�擪�̃g�[�N�������ʎq���萔���e�����łȂ����FALSE�B
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER || token->type != TOKEN_TYPE_CONSTANT)
    {
        goto END;
    }
    
    //���̃g�[�N�����A�R�����łȂ����FALSE�B
    if (parser_next(parser) == FALSE)
    {
        goto END;
    }
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_COLON)
    {
        goto END;
    }
    
    //�ϐ��錾�����B
    result = TRUE;
    
END:
    parser_returnToMark(parser, mark);
    return result;
}

