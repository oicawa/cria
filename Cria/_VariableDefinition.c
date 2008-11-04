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
    
    //�擪�̃g�[�N�������ʎq���萔���e�����łȂ����FALSE�B
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER || token->type != TOKEN_TYPE_CONSTANT)
    {
        parser_error(token);
    }
    name = token->buffer;
    
    //���̃g�[�N�����A�R�����łȂ����FALSE�B
    if (parser_next(parser) == FALSE)
    {
        parser_error(token);
    }
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_COLON)
    {
        parser_error(token);
    }
    
    
    //���̃g�[�N�������s�g�[�N���������ꍇ�̓I�u�W�F�N�g�^��NULL�ŏ������B
    if (parser_next(parser) == FALSE)
    {
        parser_error(token);
    }
    
    token = parser_getCurrent(parser);
    
    
    
    //�^�`�F�b�N
    if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        type = string_clone(token->buffer);
        if (parser_next(parser) == FALSE)
        {
            parser_error(token);
        }
    }
    
    //���s�g�[�N���ȊO�H
    if (token->type != TOKEN_TYPE_NEW_LINE)
    {
        parser_error(token);
    }
    
    definition = Memory_malloc(sizeof(struct VariableDefinitionTag));
    memset(definition, 0x00, sizeof(struct VariableDefinitionTag));
    definition->name = name;
    definition->type = type;
    
    
    //��`��o�^
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



