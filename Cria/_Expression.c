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
parserVariableExpression(
    String  name;
)
{
    VariableExpression variable = NULL;
    
    variable = Memory_malloc(sizeof(struct VariableExpressionTag));
    memset(variable, 0x00, sizeof(struct VariableExpressionTag));
    variable->name = name;
    
    return variable;
}



FunctionCallExpression
parserFunctionCallExpression(
    Parser  parser
)
{
}



Expression
parserGenerateExpression(
    Parser  parser
)
{
}



ReferenceExpression
parseReferenceExpression(
    Parser  parser
)
{
    ReferenceExpression reference = NULL;
    ReferenceExpression nextReference = NULL;
    FunctionCallExpression function = NULL;
    VariableExpression variable = NULL;
    
    Token token = NULL;
    Token next = NULL;
    
    
    //�ԋp�p��ReferenceExpression�𐶐����Ă����B
    reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
    memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
    
    
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
        reference->type = REFERENCE_TYPE_SELF;
        reference->of.variable = NULL;
        goto END;
    }
    
    
    //���ʎq�̏ꍇ
    if (token->type == TOKEN_TYPE_IDENTIFIER)
    {
        //���̃g�[�N�����s���I�h�Ȃ�I�u�W�F�N�g�Q�ƁA�����ʂȂ烁�\�b�h�Ăяo���A
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            variable = parserVariableExpression(token->buffer);
            
            reference->type = REFERENCE_TYPE_VARIABLE;
            reference->of.variable = variable;
        }
        else if (next->type == TOKEN_TYPE_PARENTHESIS_LEFT)
        {
            //���̃g�[�N���������ʂ������ꍇ�͊֐��Ăяo��
            function = functionCallExpression_parse(parser);
            
            reference->type = REFERENCE_TYPE_FUNCTION_CALL;
            reference->of.function = function;
        }
        else
        {
            //�ǂ���ł��Ȃ���Ζ߂�
            goto END;
        }
    }
    else if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        //���̃g�[�N�����s���I�h�Ȃ�A�N���X�Q�ƁA�����ʂȂ�I�u�W�F�N�g����
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            variable = parserVariableExpression(token->buffer);
            
            reference->type = REFERENCE_TYPE_VARIABLE;
            reference->of.variable = variable;
        }
        else if (next->type == TOKEN_TYPE_PARENTHESIS_LEFT)
        {
            //���̃g�[�N���������ʂ������ꍇ�͊֐��Ăяo��
            function = functionCallExpression_parse(parser);
            
            reference->type = REFERENCE_TYPE_FUNCTION_CALL;
            reference->of.function = function;
        }
        else
        {
            //�ǂ���ł��Ȃ���Ζ߂�
            goto END;
        }
    }
    
    
    //���̎Q�Ƃ��m�F
    nextReference = parserReferenceExpression(parser);
    reference->next = parserReferenceExpression(parser);
    
}



int parseFactor(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    String string = NULL;
    Token token = parser_getCurrent(parser);
    Token next = NULL;
    
    
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



int parseMulDiv(
    Parser  parser
)
{
    parseFactor(parser);
}



int parsePlusMinus(
    Parser  parser
)
{
    parseMultiplyDivide(parser);
}



int parseCompare(
    Parser  parser
)
{
    parsePlusMinus(parser);
}



int parseNot(
    Parser  parser
)
{
    parseCompare(paser);
}



int parseAndOr(
    Parser  parser
)
{
    parserNot(parser);
}



Expression
expression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    
    expression = parseAndOr(parser);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}












































