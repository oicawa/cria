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



ParametersExpression
expression_parseParametersExpression(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ParametersExpression parameters = NULL;
    Expression expression = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    List list = list_new();
    
    //右括弧のトークンが出現するまで繰り返し
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Loop start.");
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Parse expression.");
        expression = expression_parse(parser);
        if (expression == NULL)
        {
            Logger_dbg("expression is NULL.");
            parser_error(token);
            goto END;
        }
        
        //解析できたらパラメータとして追加
        Logger_dbg("Add expression.");
        list_add(list, expression);
        
        //トークンが', 'であれば更に読み出しを行って継続
        Logger_dbg("Get current token.");
        token = parser_getCurrent(parser);
        token_log(token);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            //次のトークンへ
            Logger_dbg("Token is Comma.");
            if (parser_next(parser) == FALSE)
            {
                parser_error(token);
                goto END;
            }
            token = parser_getCurrent(parser);
            continue;
        }
        
        //トークンが', 'でも右括弧でもない場合はエラー
        Logger_dbg("Token is not right parenthesis.");
        if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            token_log(token);
            parser_error(token);
            goto END;
        }
    }
    Logger_dbg("Loop end.");
    
    Logger_dbg("list count = %d", list->count);
    
    //次のトークンへ移動。
    //parser_next(parser);
    
    parameters = Memory_malloc(sizeof(struct ParametersExpressionTag));
    memset(parameters, 0x00, sizeof(struct ParametersExpressionTag));
    parameters->list = list;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return parameters;
}



ReferenceExpression
expression_parseVariable(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    ReferenceExpression expression = NULL;
    String name = NULL;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)   //TOKEN_TYPE_FUNCTION_DEFINITION
    {
        Logger_dbg("Not VariableExpression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    //変数名を保持
    name = token->buffer;
    
    //次のトークンへ
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Maybe FunctionCallExpression name = %s", name->pointer);
        parser_setPosition(parser, position);
        goto END;
    }
    
    //生成
    Logger_dbg("VariableExpression name = %s", name->pointer);
    VariableExpression variable = NULL;
    variable = Memory_malloc(sizeof(struct VariableExpressionTag));
    memset(variable, 0x00, sizeof(struct VariableExpressionTag));
    variable->name = string_clone(name);
    
    expression = Memory_malloc(sizeof(struct ReferenceExpressionTag));
    memset(expression, 0x00, sizeof(struct ReferenceExpressionTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_VARIABLE;
    expression->of.variable = variable;

    //ピリオドだった場合は次のReferenceExpressionをパース
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        expression->next = expression_parseReferenceExpression(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ReferenceExpression
expression_parseFunctionCall(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    ReferenceExpression expression = NULL;
    String name = NULL;
    ParametersExpression parameters = NULL;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)   //TOKEN_TYPE_FUNCTION_DEFINITION
    {
        parser_setPosition(parser, position);
        goto END;
    }
    
    //関数名を保持
    name = token->buffer;
    
    //次のトークンへ
    parser_next(parser);
    token = parser_getCurrent(parser);
    
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    //引数をパース
    parser_next(parser);
    parameters = expression_parseParametersExpression(parser);
    if (parameters == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    //現在のトークンを取得
    token = parser_getCurrent(parser);
    
    //右括弧でなければFALSE返却
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    //生成
    FunctionCallExpression function = Memory_malloc(sizeof(struct FunctionCallExpressionTag));
    memset(function, 0x00, sizeof(struct FunctionCallExpressionTag));
    function->name = string_clone(name);
    function->parameters = parameters;
    
    expression = Memory_malloc(sizeof(struct ReferenceExpressionTag));
    memset(expression, 0x00, sizeof(struct ReferenceExpressionTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL;
    expression->of.function = function;
    
    
    //次のトークンへ移動しておく。
    parser_next(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        expression->next = expression_parseReferenceExpression(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ReferenceExpression
expression_parseReferenceExpression(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    ReferenceExpression expression = NULL;
    
    expression = expression_parseVariable(parser);
    if (expression != NULL)
        goto END;
    
    expression = expression_parseFunctionCall(parser);
    if (expression != NULL)
        goto END;
    
    //いずれでもなかった場合はパーサー位置を戻す。
    parser_setPosition(parser, position);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
expression_parseFactor(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    Token token = parser_getCurrent(parser);
    token_log(token);
    
    
    Logger_dbg("Check integer literal.");
    if (token->type == TOKEN_TYPE_INTEGER_LITERAL)
    {
        Logger_dbg("This is an integer literal token.");
        IntegerLiteralExpression integerLiteral = Memory_malloc(sizeof(struct IntegerLiteralExpressionTag));
        memset(integerLiteral, 0x00, sizeof(struct IntegerLiteralExpressionTag));
        integerLiteral->value = string_toInteger(token->buffer);
        
        expression = expression_new(EXPRESSION_KIND_INTEGER_LITERAL);
        expression->of._integerLiteral_ = integerLiteral;
        
        parser_next(parser);
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
        
        parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check new expression.");
    if (token->type == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("This is a left parenthesis token.");
        parser_next(parser);
        expression = expression_parse(parser);
        token = parser_getCurrent(parser);
        if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            parser_error(token);
        }
        parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check reference expression.");
    if (token->type == TOKEN_TYPE_PERIOD ||
        token->type == TOKEN_TYPE_IDENTIFIER ||
        token->type == TOKEN_TYPE_CLASS_LITERAL ||
        token->type == TOKEN_TYPE_CONSTANT)
    {
        ReferenceExpression reference = expression_parseReferenceExpression(parser);
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
expression_parseMultiplyDivide(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = expression_parseFactor(parser);
    
    //次のトークンが乗算・除算のいずれかだった場合は新たにExpressionを生成
    token = parser_getCurrent(parser);
    token_log(token);
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

    //右辺の式をパース
    parser_next(parser);
    right = expression_parse(parser);
    
    //演算式を生成
    Logger_dbg("Create MultiplyDivide Expression.");
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
expression_parsePlusMinus(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = expression_parseMultiplyDivide(parser);
    
    //次のトークンが加算・減算のいずれかだった場合は新たにExpressionを生成
    token = parser_getCurrent(parser);
    token_log(token);
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

    //右辺の式をパース
    parser_next(parser);
    right = expression_parse(parser);
    
    //演算式を生成
    Logger_dbg("Create PlusMinus Expression.");
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
expression_parseCompare(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = expression_parsePlusMinus(parser);
    
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

    //右辺の式をパース
    parser_next(parser);
    right = expression_parse(parser);
    
    //演算式を生成
    Logger_dbg("Create Compare Expression.");
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
expression_parseNotEqual(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = expression_parseCompare(parser);
    
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

    //右辺の式をパース
    parser_next(parser);
    right = expression_parse(parser);
    
    //演算式を生成
    Logger_dbg("Create NotEqual Expression.");
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
expression_parseAndOr(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    OperationExpression operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = expression_parseNotEqual(parser);
    
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

    //右辺の式をパース
    parser_next(parser);
    right = expression_parse(parser);
    
    //演算式を生成
    Logger_dbg("Create AndOr Expression.");
    operation = Memory_malloc(sizeof(struct OperationExpressionTag));
    memset(operation, 0x00, sizeof(struct OperationExpressionTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
expression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    
    expression = expression_parseAndOr(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}








































