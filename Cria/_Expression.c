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



VariableExpression
parseVariableExpression(
    Parser  parser,
    String  name
)
{
    Logger_trc("[ START ]%s", __func__);
    VariableExpression variable = NULL;
    
    variable = Memory_malloc(sizeof(struct VariableExpressionTag));
    memset(variable, 0x00, sizeof(struct VariableExpressionTag));
    variable->name = name;
    
    parser_next(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return variable;
}



ClassExpression
parseClassExpression(
    Parser  parser,
    String  name
)
{
    Logger_trc("[ START ]%s", __func__);
    ClassExpression expression = NULL;
    
    expression = Memory_malloc(sizeof(struct ClassExpressionTag));
    memset(expression, 0x00, sizeof(struct ClassExpressionTag));
    expression->name = name;
    
    parser_next(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ParametersExpression
expression_parseParametersExpression(
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
        
        /*
        //次のトークンへ
        Logger_dbg("Next token.");
        if (parser_next(parser) == FALSE)
        {
            parser_error(token);
            goto END;
        }
        */
        
        //トークンが', 'であれば更に読み出しを行って継続
        Logger_dbg("Get current token.");
        token = parser_getCurrent(parser);
        token_log(token);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            //次のトークンへ
            Logger_dbg("Token is not Comma.");
            if (parser_next(parser) == FALSE)
            {
                parser_error(token);
                goto END;
            }
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
    token = parser_getCurrent(parser);
    
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
    parametersExpression = expression_parseParametersExpression(parser);
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
    
    
    //次のトークンへ移動しておく。
    parser_next(parser);
    
    
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
    parametersExpression = expression_parseParametersExpression(parser);
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
    
    
    //次のトークンへ移動しておく。
    parser_next(parser);
    
    
    expression = Memory_malloc(sizeof(struct FunctionCallExpressionTag));
    memset(expression, 0x00, sizeof(struct FunctionCallExpressionTag));
    expression->name = className;
    expression->parameters = parametersExpression;
    
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
    ReferenceExpression reference = NULL;
    FunctionCallExpression function = NULL;
    VariableExpression variable = NULL;
    ClassExpression klass = NULL;
    GenerateExpression generate = NULL;
    
    Token token = NULL;
    Token next = NULL;
    
    
    //現在のトークンと次のトークンへの参照を取得
    Logger_dbg("Get current token.");
    token = parser_getCurrent(parser);
    token_log(token);
    if (token == NULL)
    {
        Logger_dbg("Token is NULL.");
        goto END;
    }
    
    
    next = parser_getNext(parser);
    
    
    //ピリオドの場合（自己インスタンス参照）
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Logger_dbg("Token is period.");
        //次のトークンからの解析
        if (parser_next(parser) == FALSE)
        {
            parser_error(next);
            goto END;
        }
        
        //先頭のトークンがピリオドだった場合は自己インスタンス、またはクラスへの参照とみなす。
        Logger_dbg("Create self reference.");
        reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
        memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
        reference->type = REFERENCE_EXPRESSION_TYPE_SELF;
        reference->of.variable = NULL;
        goto NEXT_REFERENCE;
    }
    
    
    //識別子の場合
    if (token->type == TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Token is Identifier.");
        //次のトークンがピリオドならオブジェクト参照、左括弧ならメソッド呼び出し、
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            Logger_dbg("Create variable reference.");
            variable = parseVariableExpression(parser, token->buffer);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_EXPRESSION_TYPE_VARIABLE;
            reference->of.variable = variable;
            
            parser_next(parser);
            
            goto NEXT_REFERENCE;
        }
        else if (next->type == TOKEN_TYPE_PARENTHESIS_LEFT)
        {
            Logger_dbg("Create function call reference.");
            function = parseFunctionCallExpression(parser);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL;
            reference->of.function = function;
            
            token_log(parser_getCurrent(parser));
            
            goto NEXT_REFERENCE;
        }
        else
        {
            Logger_dbg("Create variable reference.");
            variable = parseVariableExpression(parser, token->buffer);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_EXPRESSION_TYPE_VARIABLE;
            reference->of.variable = variable;
            
            Logger_dbg("End of reference..");
            goto END;
        }
    }
    else if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("Token is Class literal.");
        if (next->type == TOKEN_TYPE_PERIOD)
        {
            Logger_dbg("Create class reference.");
            klass = parseClassExpression(parser, token->buffer);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_EXPRESSION_TYPE_CLASS;
            reference->of.klass = klass;
            
            goto NEXT_REFERENCE;
        }
        else if (next->type == TOKEN_TYPE_PARENTHESIS_LEFT)
        {
            Logger_dbg("Create generate reference.");
            generate = parseGenerateExpression(parser);
            
            reference = Memory_malloc(sizeof(struct ReferenceExpressionTag));
            memset(reference, 0x00, sizeof(struct ReferenceExpressionTag));
            reference->type = REFERENCE_EXPRESSION_TYPE_GENERATE;
            reference->of.generate = generate;
            
            goto NEXT_REFERENCE;
        }
        else
        {
            Logger_dbg("No match reference expression.");
            parser_error(next);
            goto END;
        }
    }
    
    //上記いずれのパターンにも当てはまらない場合はNULLを返却
    goto END;
    
NEXT_REFERENCE:
    //次の参照を確認
    Logger_dbg("Create next reference expression.");
    reference->next = expression_parseReferenceExpression(parser);
    
END:
    Logger_trc("[  END  ]%s", __func__);
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
parseMultiplyDivide(
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
    
    left = parseFactor(parser);
    /*
    if (parser_next(parser) == FALSE)
    {
        //parser_error(parser_getCurrent(parser));
        expression = left;
        goto END;
    }
    */
    
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

    //右辺開始位置へ移動
    parser_next(parser);
    
    //右辺の式をパース
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
parsePlusMinus(
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
    
    left = parseMultiplyDivide(parser);
    
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

    //右辺開始位置へ移動
    parser_next(parser);
    
    //右辺の式をパース
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
parseCompare(
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
    
    left = parsePlusMinus(parser);
    /*
    if (parser_next(parser) == FALSE)
    {
        //parser_error(parser_getCurrent(parser));
        expression = left;
        goto END;
    }
    */
    
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

    //右辺開始位置へ移動
    parser_next(parser);
    
    //右辺の式をパース
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
parseNotEqual(
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
    
    left = parseCompare(parser);
    /*
    if (parser_next(parser) == FALSE)
    {
        //parser_error(parser_getCurrent(parser));
        expression = left;
        goto END;
    }
    */
    
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

    //右辺開始位置へ移動
    parser_next(parser);
    
    //右辺の式をパース
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
parseAndOr(
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
    
    left = parseNotEqual(parser);
    /*
    if (parser_next(parser) == FALSE)
    {
        //parser_error(parser_getCurrent(parser));
        expression = left;
        goto END;
    }
    */
    
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

    //右辺開始位置へ移動
    parser_next(parser);
    
    //右辺の式をパース
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
    
    expression = parseAndOr(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}








































