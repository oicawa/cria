#include <stdio.h>

#include "_Cria.h"



Expression
expression_new(
    ExpressionKind  kind
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = Memory_malloc(sizeof(struct ExpressionTag));
    memset(expression, 0x00, sizeof(struct ExpressionTag));
    expression->kind = kind;
    Logger_trc("expression kind is '%d'", expression->kind);
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



void
expression_dispose(
    Expression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    if (expression == NULL)
    {
        goto END;
    }
    
    switch (expression->kind)
    {
    case EXPRESSION_KIND_OPERATION:
        //operationExpression_dispose(expression->of._operation_);
        break;
    case EXPRESSION_KIND_STRING_LITERAL:
        stringLiteralExpression_dispose(expression->of._stringLiteral_);
        expression->of._stringLiteral_ = NULL;
        break;
    case EXPRESSION_KIND_INTEGER_LITERAL:
        break;
    case EXPRESSION_KIND_REAL_LITERAL:
        break;
    case EXPRESSION_KIND_BOOLEAN_LITERAL:
        break;
    case EXPRESSION_KIND_NULL_LITERAL:
        break;
    case EXPRESSION_KIND_GENERATE:
        break;
    case EXPRESSION_KIND_FUNCTION_CALL:
        //functionCallExpression_dispose(expression->of._functionCall_);
        break;
    case EXPRESSION_KIND_VARIABLE:
        break;
    default:
        Logger_err("Illegal statement type. (%d)", expression->kind);
        break;
    }
    Memory_free(expression);
    expression = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



VariableExpression
parseVariableExpression(
    String  name
)
{
    VariableExpression variable = NULL;
    
    variable = Memory_malloc(sizeof(struct VariableExpressionTag));
    memset(variable, 0x00, sizeof(struct VariableExpressionTag));
    variable->name = name;
    
    return variable;
}



ClassExpression
parseClassExpression(
    String  name
)
{
    ClassExpression expression = NULL;
    
    expression = Memory_malloc(sizeof(struct ClassExpressionTag));
    memset(expression, 0x00, sizeof(struct ClassExpressionTag));
    expression->name = name;
    
    return expression;
}



ParametersExpression
parseParametersExpression(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ParametersExpression parameters = NULL;
    Expression expression = NULL;
    Token token = NULL;
    List list = list_new();
    
    //�E���ʂ̃g�[�N�����o������܂ŌJ��Ԃ�
    token = parser_getCurrent(parser);
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        //����́B
        expression = expression_parse(parser);
        if (expression == NULL)
        {
            Logger_dbg("Not expression.");
            goto END;
        }
        
        //��͂ł�����p�����[�^�Ƃ��Ēǉ�
        list_add(list, expression);
        
        //���̃g�[�N����
        if (parser_next(parser) == FALSE)
        {
            Logger_err("Next token is nothing.");
            perror("Next token is nothing.");
            goto END;
        }
        
        //�g�[�N����', '�ł���΍X�ɓǂݏo�����s���Čp��
        token = parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            //���̃g�[�N����
            if (parser_next(parser) == FALSE)
            {
                Logger_err("Next token is nothing.");
                perror("Next token is nothing.");
                goto END;
            }
            continue;
        }
        
        //�g�[�N����', '�ł��E���ʂł��Ȃ��ꍇ�̓G���[
        if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            Logger_err("Next token is illegal.");
            perror("Next token is illegal.");
            goto END;
        }
    }
    
    parameters = Memory_malloc(sizeof(struct ParametersExpressionTag));
    memset(parameters, 0x00, sizeof(struct ParametersExpressionTag));
    parameters->list = list;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return parameters;
}



FunctionCallExpression
parseFunctionCallExpression(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = parser_getCurrent(parser);
    FunctionCallExpression expression = NULL;
    String functionName = NULL;
    ParametersExpression parametersExpression = NULL;
    
    
    //�ŏ��̃g�[�N����]�����A���ʎq�łȂ����NULL�ԋp�B
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("First token is not identifier.");
        goto END;
    }
    
    //OK�Ȃ环�ʎq���֐����Ƃ��Ď擾
    functionName = string_clone(token->buffer);
    
    //���̃g�[�N�����Ȃ����NULL��ԋp
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Second token does not exist.");
        goto END;
    }
    
    //����΂�����擾
    Logger_dbg("Get second token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got second token.");
    
    //�����ʂłȂ����FALSE�ԋp
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        goto END;
    }
    
    //���̃g�[�N�����Ȃ����FALSE��ԋp
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Third token does not exist.");
        goto END;
    }
    
    //�������p�[�X
    parametersExpression = parseParametersExpression(parser);
    if (parametersExpression == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        goto END;
    }
    
    //���݂̃g�[�N�����擾
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    
    //�E���ʂłȂ����FALSE�ԋp
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    expression = Memory_malloc(sizeof(struct FunctionCallExpressionTag));
    memset(expression, 0x00, sizeof(struct FunctionCallExpressionTag));
    expression->name = functionName;
    expression->parameters = parametersExpression;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



GenerateExpression
parseGenerateExpression(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = parser_getCurrent(parser);
    GenerateExpression expression = NULL;
    String className = NULL;
    ParametersExpression parametersExpression = NULL;
    
    
    //�ŏ��̃g�[�N����]�����A�N���X���e�����łȂ����NULL�ԋp�B
    if (token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("First token is not class literal.");
        goto END;
    }
    
    //OK�Ȃ环�ʎq���֐����Ƃ��Ď擾
    className = string_clone(token->buffer);
    
    //���̃g�[�N�����Ȃ����NULL��ԋp
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Second token does not exist.");
        goto END;
    }
    
    //����΂�����擾
    Logger_dbg("Get second token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got second token.");
    
    //�����ʂłȂ����FALSE�ԋp
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        goto END;
    }
    
    //���̃g�[�N�����Ȃ����FALSE��ԋp
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Third token does not exist.");
        goto END;
    }
    
    //�������p�[�X
    parametersExpression = parseParametersExpression(parser);
    if (parametersExpression == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        goto END;
    }
    
    //���݂̃g�[�N�����擾
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    
    //�E���ʂłȂ����FALSE�ԋp
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    expression = Memory_malloc(sizeof(struct FunctionCallExpressionTag));
    memset(expression, 0x00, sizeof(struct FunctionCallExpressionTag));
    expression->name = className;
    expression->parameters = parametersExpression;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ReferenceExpression
parseReferenceExpression(
    Parser  parser
)
{
    ReferenceExpression reference = NULL;
    FunctionCallExpression function = NULL;
    VariableExpression variable = NULL;
    ClassExpression klass = NULL;
    GenerateExpression generate = NULL;
    
    Token token = NULL;
    Token next = NULL;
    
    
    //���݂̃g�[�N���Ǝ��̃g�[�N���ւ̎Q�Ƃ��擾
    token = parser_getCurrent(parser);
    next = parser_getNext(parser);
    
    
    //�s���I�h�̏ꍇ�i���ȃC���X�^���X�Q�Ɓj
    Logger_dbg("Check identifier expression.");
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        //���̃g�[�N������̉��
        if (parser_next(parser) == FALSE)
        {
            parser_error(next);
            goto END;
        }
        
        //�擪�̃g�[�N�����s���I�h�������ꍇ�͎��ȃC���X�^���X�A�܂��̓N���X�ւ̎Q�ƂƂ݂Ȃ��B
        reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
        memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
        reference->type = REFERENCE_TYPE_SELF;
        reference->of.variable = NULL;
        goto NEXT_REFERENCE;
    }
    
    
    //���ʎq�̏ꍇ
    if (token->type == TOKEN_TYPE_IDENTIFIER)
    {
        //���̃g�[�N�����s���I�h�Ȃ�I�u�W�F�N�g�Q�ƁA�����ʂȂ烁�\�b�h�Ăяo���A
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            variable = parseVariableExpression(token->buffer);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_TYPE_VARIABLE;
            reference->of.variable = variable;
        }
        else if (next->type == TOKEN_TYPE_PARENTHESIS_LEFT)
        {
            //���̃g�[�N���������ʂ������ꍇ�͊֐��Ăяo��
            function = parseFunctionCallExpression(parser);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_TYPE_FUNCTION_CALL;
            reference->of.function = function;
        }
        else
        {
            //�ǂ���ł��Ȃ���Εϐ��ŏI�[
            goto END;
        }
        goto NEXT_REFERENCE;
    }
    else if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        //���̃g�[�N�����s���I�h�Ȃ�A�N���X�Q�ƁA�����ʂȂ�I�u�W�F�N�g����
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            klass = parseClassExpression(token->buffer);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_TYPE_CLASS;
            reference->of.klass = klass;
        }
        else if (next->type == TOKEN_TYPE_PARENTHESIS_LEFT)
        {
            //���̃g�[�N���������ʂ������ꍇ�͊֐��Ăяo��
            generate = parseGenerateExpression(parser);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_TYPE_GENERATE;
            reference->of.generate = generate;
        }
        else
        {
            //�ǂ���ł��Ȃ���΃G���[
            parser_error(next);
            goto END;
        }
        goto NEXT_REFERENCE;
    }
    
    //��L������̃p�^�[���ɂ����Ă͂܂�Ȃ��ꍇ��NULL��ԋp
    goto END;
    
NEXT_REFERENCE:
    //���̎Q�Ƃ��m�F
    reference->next = parseReferenceExpression(parser);
    
END:
    return reference;
}



Expression
parseFactor(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    Token token = parser_getCurrent(parser);
    
    
    Logger_dbg("Check integer literal.");
    if (token->type == TOKEN_TYPE_INTEGER_LITERAL)
    {
        Logger_dbg("This is an integer literal token.");
        IntegerLiteralExpression integerLiteral = Memory_malloc(sizeof(struct IntegerLiteralExpressionTag));
        memset(integerLiteral, 0x00, sizeof(struct IntegerLiteralExpressionTag));
        integerLiteral->value = string_toInteger(token->buffer);
        
        expression = expression_new(EXPRESSION_KIND_STRING_LITERAL);
        expression->of._integerLiteral_ = integerLiteral;
        goto END;
    }
    
    
    Logger_dbg("Check string literal.");
    if (token->type == TOKEN_TYPE_STRING_LITERAL)
    {
        Logger_dbg("This is a string literal token.");
        StringLiteralExpression stringLiteral = Memory_malloc(sizeof(struct StringLiteralExpressionTag));
        memset(stringLiteral, 0x00, sizeof(struct StringLiteralExpressionTag));
        stringLiteral->value = string_clone(token->buffer);
        
        expression = expression_new(EXPRESSION_KIND_STRING_LITERAL);
        expression->of._stringLiteral_ = stringLiteral;
        goto END;
    }
    
    
    Logger_dbg("Check new expression.");
    if (token->type == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("This is a left parenthesis token.");
        expression = expression_parse(parser);
        goto END;
    }
    
    
    Logger_dbg("Check reference expression.");
    if (token->type == TOKEN_TYPE_PERIOD ||
        token->type == TOKEN_TYPE_IDENTIFIER ||
        token->type == TOKEN_TYPE_CLASS_LITERAL ||
        token->type == TOKEN_TYPE_CONSTANT)
    {
        ReferenceExpression reference = parseReferenceExpression(parser);
        if (reference == NULL)
        {
            Logger_err("Not reference expression.");
            goto END;
        }
        
        expression = expression_new(EXPRESSION_KIND_REFERENCE);
        expression->of._reference_ = reference;
        goto END;
    }
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
parseMultiplyDivide(
    Parser  parser
)
{
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parseFactor(parser);
    if (parser_next(parser) == FALSE)
    {
        parser_error(parser_getCurrent(parser));
        goto END;
    }
    
    
    //���̃g�[�N������Z�E���Z�̂����ꂩ�������ꍇ�͐V����Expression�𐶐�
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_MULTIPLY)
    {
        kind = OPERATION_KIND_MULTIPLY;
    }
    else if (token->type == TOKEN_TYPE_DEVIDE)
    {
        kind = OPERATION_KIND_DIVIDE;
    }
    else
    {
        expression = left;
        goto END;
    }

    //�E�ӂ̎����p�[�X
    right = expression_parse(parser);
    
    //���Z���𐶐�
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    return expression;
}



Expression
parsePlusMinus(
    Parser  parser
)
{
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parseMultiplyDivide(parser);
    if (parser_next(parser) == FALSE)
    {
        parser_error(parser_getCurrent(parser));
        goto END;
    }
    
    
    //���̃g�[�N�������Z�E���Z�̂����ꂩ�������ꍇ�͐V����Expression�𐶐�
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PLUS)
    {
        kind = OPERATION_KIND_PLUS;
    }
    else if (token->type == TOKEN_TYPE_MINUS)
    {
        kind = OPERATION_KIND_MINUS;
    }
    else
    {
        expression = left;
        goto END;
    }

    //�E�ӂ̎����p�[�X
    right = expression_parse(parser);
    
    //���Z���𐶐�
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    return expression;
}



Expression
parseCompare(
    Parser  parser
)
{
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parsePlusMinus(parser);
    if (parser_next(parser) == FALSE)
    {
        parser_error(parser_getCurrent(parser));
        goto END;
    }
    
    
    //���̃g�[�N�������Z�E���Z�̂����ꂩ�������ꍇ�͐V����Expression�𐶐�
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_EQUAL)
    {
        kind = OPERATION_KIND_EQUAL;
    }
    else if (token->type == TOKEN_TYPE_LESS_EQUAL)
    {
        kind = OPERATION_KIND_LESS_EQUAL;
    }
    else if (token->type == TOKEN_TYPE_LESS_THAN)
    {
        kind = OPERATION_KIND_LESS_THAN;
    }
    else
    {
        expression = left;
        goto END;
    }

    //�E�ӂ̎����p�[�X
    right = expression_parse(parser);
    
    //���Z���𐶐�
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    return expression;
}



Expression
parseNot(
    Parser  parser
)
{
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parseCompare(parser);
    if (parser_next(parser) == FALSE)
    {
        parser_error(parser_getCurrent(parser));
        goto END;
    }
    
    
    //���̃g�[�N�������Z�E���Z�̂����ꂩ�������ꍇ�͐V����Expression�𐶐�
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_NOT_EQUAL)
    {
        kind = OPERATION_KIND_NOT_EQUAL;
    }
    else
    {
        expression = left;
        goto END;
    }

    //�E�ӂ̎����p�[�X
    right = expression_parse(parser);
    
    //���Z���𐶐�
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    return expression;
}



Expression
parseAndOr(
    Parser  parser
)
{
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parseNot(parser);
    if (parser_next(parser) == FALSE)
    {
        parser_error(parser_getCurrent(parser));
        goto END;
    }
    
    
    //���̃g�[�N�������Z�E���Z�̂����ꂩ�������ꍇ�͐V����Expression�𐶐�
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_OR)
    {
        kind = OPERATION_KIND_OR;
    }
    else if (token->type == TOKEN_TYPE_AND)
    {
        kind = OPERATION_KIND_AND;
    }
    else
    {
        expression = left;
        goto END;
    }

    //�E�ӂ̎����p�[�X
    right = expression_parse(parser);
    
    //���Z���𐶐�
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    return expression;
}



Expression
expression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    
    expression = parseAndOr(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}












































