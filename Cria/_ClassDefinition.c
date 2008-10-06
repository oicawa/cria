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
    
    //NULL�`�F�b�N
    if (parser == NULL)
    {
        goto END;
    }
    
    //���݂̃g�[�N���𕜋A�ʒu�Ƃ��ăo�b�N�A�b�v
    mark = parser->current;
    
    //�擪�̃g�[�N�����N���X���e�����łȂ����FALSE�B
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
        goto END;
    }
    
    //���̃g�[�N���ցB
    if (parser_next(parser) == FALSE)
    {
        goto END;
    }
    token = parser_getCurrent(parser);
    
    //���̃g�[�N�������s�g�[�N�����A�p���g�[�N���̏ꍇ��OK
    if (token->type == TOKEN_TYPE_NEW_LINE || token->type == TOKEN_TYPE_EXTENDS)
    {
        result = TRUE;
        goto END;
    }
    
    
END:
    parser_returnToMark(parser, mark);
    return result;
}

