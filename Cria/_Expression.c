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
    
    //右括弧のトークンが出現するまで繰り返し
    token = parser_getCurrent(parser);
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        //式解析。
        expression = expression_parse(parser);
        if (expression == NULL)
        {
            Logger_dbg("Not expression.");
            goto END;
        }
        
        //解析できたらパラメータとして追加
        list_add(list, expression);
        
        //次のトークンへ
        if (parser_next(parser) == FALSE)
        {
            Logger_err("Next token is nothing.");
            perror("Next token is nothing.");
            goto END;
        }
        
        //トークンが', 'であれば更に読み出しを行って継続
        token = parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            //次のトークンへ
            if (parser_next(parser) == FALSE)
            {
                Logger_err("Next token is nothing.");
                perror("Next token is nothing.");
                goto END;
            }
            continue;
        }
        
        //トークンが', 'でも右括弧でもない場合はエラー
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
    
    
    //最初のトークンを評価し、識別子でなければNULL返却。
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("First token is not identifier.");
        goto END;
    }
    
    //OKなら識別子を関数名として取得
    functionName = string_clone(token->buffer);
    
    //次のトークンがなければNULLを返却
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Second token does not exist.");
        goto END;
    }
    
    //あればそれを取得
    Logger_dbg("Get second token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got second token.");
    
    //左括弧でなければFALSE返却
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        goto END;
    }
    
    //次のトークンがなければFALSEを返却
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Third token does not exist.");
        goto END;
    }
    
    //引数をパース
    parametersExpression = parseParametersExpression(parser);
    if (parametersExpression == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        goto END;
    }
    
    //現在のトークンを取得
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    
    //右括弧でなければFALSE返却
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
    
    
    //最初のトークンを評価し、クラスリテラルでなければNULL返却。
    if (token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("First token is not class literal.");
        goto END;
    }
    
    //OKなら識別子を関数名として取得
    className = string_clone(token->buffer);
    
    //次のトークンがなければNULLを返却
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Second token does not exist.");
        goto END;
    }
    
    //あればそれを取得
    Logger_dbg("Get second token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got second token.");
    
    //左括弧でなければFALSE返却
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        goto END;
    }
    
    //次のトークンがなければFALSEを返却
    if (parser_next(parser) == FALSE)
    {
        Logger_dbg("Third token does not exist.");
        goto END;
    }
    
    //引数をパース
    parametersExpression = parseParametersExpression(parser);
    if (parametersExpression == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        goto END;
    }
    
    //現在のトークンを取得
    Logger_dbg("Get last token.");
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Got last token.");
    
    //右括弧でなければFALSE返却
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
    
    
    //現在のトークンと次のトークンへの参照を取得
    token = parser_getCurrent(parser);
    next = parser_getNext(parser);
    
    
    //ピリオドの場合（自己インスタンス参照）
    Logger_dbg("Check identifier expression.");
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        //次のトークンからの解析
        if (parser_next(parser) == FALSE)
        {
            parser_error(next);
            goto END;
        }
        
        //先頭のトークンがピリオドだった場合は自己インスタンス、またはクラスへの参照とみなす。
        reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
        memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
        reference->type = REFERENCE_TYPE_SELF;
        reference->of.variable = NULL;
        goto NEXT_REFERENCE;
    }
    
    
    //識別子の場合
    if (token->type == TOKEN_TYPE_IDENTIFIER)
    {
        //次のトークンがピリオドならオブジェクト参照、左括弧ならメソッド呼び出し、
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
            //次のトークンが左括弧だった場合は関数呼び出し
            function = parseFunctionCallExpression(parser);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_TYPE_FUNCTION_CALL;
            reference->of.function = function;
        }
        else
        {
            //どちらでもなければ変数で終端
            goto END;
        }
        goto NEXT_REFERENCE;
    }
    else if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        //次のトークンがピリオドなら、クラス参照、左括弧ならオブジェクト生成
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
            //次のトークンが左括弧だった場合は関数呼び出し
            generate = parseGenerateExpression(parser);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_TYPE_GENERATE;
            reference->of.generate = generate;
        }
        else
        {
            //どちらでもなければエラー
            parser_error(next);
            goto END;
        }
        goto NEXT_REFERENCE;
    }
    
    //上記いずれのパターンにも当てはまらない場合はNULLを返却
    goto END;
    
NEXT_REFERENCE:
    //次の参照を確認
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
    
    
    //次のトークンが乗算・除算のいずれかだった場合は新たにExpressionを生成
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

    //右辺の式をパース
    right = expression_parse(parser);
    
    //演算式を生成
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
    
    
    //次のトークンが加算・減算のいずれかだった場合は新たにExpressionを生成
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

    //右辺の式をパース
    right = expression_parse(parser);
    
    //演算式を生成
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
    
    
    //次のトークンが加算・減算のいずれかだった場合は新たにExpressionを生成
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
    right = expression_parse(parser);
    
    //演算式を生成
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
    
    
    //次のトークンが加算・減算のいずれかだった場合は新たにExpressionを生成
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
    right = expression_parse(parser);
    
    //演算式を生成
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
    
    
    //次のトークンが加算・減算のいずれかだった場合は新たにExpressionを生成
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
    right = expression_parse(parser);
    
    //演算式を生成
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












































