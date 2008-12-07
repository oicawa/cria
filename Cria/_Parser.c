#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_Tokenizer.h"
#include "_DefinitionVariable.h"
#include "_DefinitionFunction.h"
#include "_Statement.h"
#include "_Reference.h"
#include "_Expression.h"

#include "_Parser.h"





void
parser_errorFunction(
	Token token,
	char* file,
	int line
)
{
	Logger_err("Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", token->buffer, token->row, token->column, file, line);
	fprintf(stderr, "Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", token->buffer, token->row, token->column, file, line);
	Memory_dispose();
	exit(1);
}


Parser
parser_new(
    List    list
)
{
    Logger_trc("[ START ]%s", __func__);
    Parser parser = Memory_malloc(sizeof(struct ParserTag));
    memset(parser, 0x00, sizeof(struct ParserTag));
    
    parser->tokens = list;
    parser->current = NULL;
    parser->next = list->item;
    parser->mark = NULL;
    
    Logger_trc("[  END  ]%s", __func__);
    return parser;
}



void
parser_dispose(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Logger_dbg("Check parser.");
    if (parser == NULL)
    {
        Logger_dbg("parser is NULL.");
        goto END;
    }
    
    Logger_dbg("Free parser.");
    Memory_free(parser);
    Logger_dbg("Set NULL to parser.");
    parser = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



Token
parser_getCurrent(
    Parser  parser
)
{
    Logger_dbg("[ START ]%s", __func__);
    if (parser == NULL)
    {
        Logger_dbg("parser is NULL.");
        return NULL;
    }
    Logger_dbg("parser is not NULL.");
    Item item = parser->current;
    if (item == NULL)
    {
        Logger_dbg("item is NULL.");
        return NULL;
    }
    Logger_dbg("item is not NULL.");
    Token token = (Token)(item->object);
    if (token == NULL)
    {
        Logger_dbg("token is NULL.");
        return NULL;
    }
    Logger_dbg("token is not NULL.");
    
    Logger_dbg("[  END  ]%s", __func__);
    return token;
}



Boolean
parser_next(
    Parser  parser
)
{
    if (parser == NULL)
    {
        Logger_dbg("Parser is NULL.");
        return FALSE;
    }
    
    
    if (parser->next == NULL)
    {
        Logger_dbg("Next token is  NULL.");
        return FALSE;
    }
    
    
    Item current = parser->next;
    token_log(((Token)(current->object)));
    parser->current = current;
    parser->next = current->next;
    
    return TRUE;
}



Item
parser_getPosition(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    if (parser == NULL)
        return NULL;
    
    Logger_trc("[  END  ]%s", __func__);
    return parser->current;
}



void
parser_setPosition(
    Parser  parser,
    Item    position
)
{
    Logger_trc("[ START ]%s", __func__);
    if (parser == NULL)
        return;
    
    parser->current = position;
    
    if (position == NULL)
        parser->next = parser->tokens->item;
    else
        parser->next = position->next;
    
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
parser_eat(
	Parser parser,
	TokenType type,
	Boolean isNessesally
)
{
	Boolean result = FALSE;
	Token token = parser_getCurrent(parser);
	if (token == NULL)
		goto UNMATCH;
	
	if (token->type != type)
		goto UNMATCH;
	
	result = TRUE;
	parser_next(parser);
	goto END;

UNMATCH:
	if (isNessesally == TRUE)
		parser_error(token);
	
END:
	return result;
}



Boolean
parser_is_end(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = NULL;
    Boolean result = FALSE;
    
    while (TRUE)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token == NULL)
        {
            result = TRUE;
            goto END;
        }
        
        if (token->type != TOKEN_TYPE_NEW_LINE)
            goto END;
        
        if (parser_next(parser) == FALSE)
        {
            result = TRUE;
            break;
        }
    }
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



ExpressionParameters
parser_expression_parameters(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionParameters parameters = NULL;
    Expression expression = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    List list = list_new();
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    Logger_dbg("Loop start.");
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Parse expression.");
        expression = parser_expression(parser);
        if (expression == NULL)
        {
            Logger_dbg("expression is NULL.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        
        Logger_dbg("Add expression.");
        list_add(list, expression);
        
        
        Logger_dbg("Get current token.");
        token = parser_getCurrent(parser);
        token_log(token);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            Logger_dbg("Token is Comma.");
            if (parser_next(parser) == FALSE)
            {
                parser_setPosition(parser, position);
                parser_error(token);
                goto END;
            }
            token = parser_getCurrent(parser);
            continue;
        }
        
        
        Logger_dbg("Token is not right parenthesis.");
        if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            token_log(token);
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
    }
    Logger_dbg("Loop end.");
    
    Logger_dbg("list count = %d", list->count);
    
    
    parameters = Memory_malloc(sizeof(struct ExpressionParametersTag));
    memset(parameters, 0x00, sizeof(struct ExpressionParametersTag));
    parameters->list = list;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return parameters;
}



Reference
parser_reference_variable(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Reference reference = NULL;
    ReferenceVariable variable = NULL;
    Item position = parser_getPosition(parser);
    String name = NULL;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
    	token_log(token);
    	Logger_dbg("Not identifier.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->buffer;
	Logger_dbg("Variable name is '%s'", name);
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD && token->type != TOKEN_TYPE_SUBSTITUTE)
    {
    	token_log(token);
    	Logger_dbg("Not '.' and ' = '");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    memset(variable, 0x00, sizeof(struct ReferenceVariableTag));
    variable->name = string_clone(name);
	Logger_dbg("Created ReferenceVariable");

    
    reference = reference_new(REFERENCE_TYPE_VARIABLE);
    reference->of.variable = variable;
    
    
    if (token->type != TOKEN_TYPE_PERIOD)
    {
    	token_log(token);
    	Logger_dbg("Not '.'");
        goto END;
    }
    
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    token_log(token);
    reference->next = parser_reference(parser);
    if (reference->next == NULL)
        parser_error(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
parser_reference_functionCall(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    Reference reference = NULL;
    ReferenceFunctionCall functionCall = NULL;
    ExpressionParameters parameters = NULL;
    String name = NULL;
    
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
    	token_log(token);
	    Logger_dbg("Not identifier.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->buffer;
    Logger_dbg("function name is '%s'", name);
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
    	token_log(token);
	    Logger_dbg("Not '('.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    parser_next(parser);
    parameters = parser_expression_parameters(parser);
    if (parameters == NULL)
    {
	    Logger_dbg("Not parameters.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
    	token_log(token);
	    Logger_dbg("Not ')'.");
    	parser_error(token);
        goto END;
    }
    
    functionCall = Memory_malloc(sizeof(struct ReferenceFunctionCallTag));
    memset(functionCall, 0x00, sizeof(struct ReferenceFunctionCallTag));
    functionCall->name = string_clone(name);
    functionCall->parameters = parameters;
    Logger_dbg("Created ReferenceFunctionCall");

    reference = reference_new(REFERENCE_TYPE_FUNCTION_CALL);
    reference->of.function = functionCall;
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD)
    {
    	token_log(token);
	    Logger_dbg("Not '.'.");
    	goto END;
    }
    
	parser_next(parser);
	reference->next = parser_reference(parser);
	if (reference->next == NULL)
	{
	    Logger_dbg("Not exist next Reference after '.'");
		parser_error(token);
	}
	
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
parser_reference(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    Reference reference = NULL;
    
    
    reference = parser_reference_variable(parser);
    if (reference != NULL)
        goto END;
    
    reference = parser_reference_functionCall(parser);
    if (reference != NULL)
        goto END;
    
    //reference = reference_class(parser);
    //if (reference != null)
    //    goto END;
        
    
    parser_setPosition(parser, position);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



ExpressionReference
parser_expression_variable(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    ExpressionReference expression = NULL;
    String name = NULL;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Not VariableExpression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->buffer;
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Maybe FunctionCallExpression name = %s", name);
        parser_setPosition(parser, position);
        goto END;
    }
    
    Logger_dbg("VariableExpression name = %s", name);
    ExpressionVariable variable = NULL;
    variable = Memory_malloc(sizeof(struct ExpressionVariableTag));
    memset(variable, 0x00, sizeof(struct ExpressionVariableTag));
    variable->name = string_clone(name);
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_VARIABLE;
    expression->of.variable = variable;

	token_log(token);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
    	Logger_dbg("Next ReferenceExprssion parse.");
    	parser_next(parser);
        expression->next = parser_expression_reference(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ExpressionReference
parser_expression_functionCall(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    ExpressionReference expression = NULL;
    String name = NULL;
    ExpressionParameters parameters = NULL;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    name = token->buffer;
    
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    parser_next(parser);
    parameters = parser_expression_parameters(parser);
    if (parameters == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    
    
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    ExpressionFunctionCall function = Memory_malloc(sizeof(struct ExpressionFunctionCallTag));
    memset(function, 0x00, sizeof(struct ExpressionFunctionCallTag));
    function->name = string_clone(name);
    function->parameters = parameters;
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL;
    expression->of.function = function;
    
    
    parser_next(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        expression->next = parser_expression_reference(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ExpressionReference
parser_expression_generate(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    ExpressionReference expression = NULL;
    String name = NULL;
    ExpressionParameters parameters = NULL;
    
    Token token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
    	token_log(token);
    	Logger_dbg("Not class literal.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    name = token->buffer;
    Logger_dbg("Class name is '%s'", name);
    
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
    	token_log(token);
        Logger_dbg("Second token is not left parenthesis.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    parser_next(parser);
    parameters = parser_expression_parameters(parser);
    if (parameters == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    
    
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
    	token_log(token);
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    ExpressionGenerate generate = Memory_malloc(sizeof(struct ExpressionGenerateTag));
    memset(generate, 0x00, sizeof(struct ExpressionGenerateTag));
    generate->name = string_clone(name);
    generate->parameters = parameters;
    Logger_dbg("Created GenerateExpression");
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_GENERATE;
    expression->of.generate = generate;
    
    
    parser_next(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        expression->next = parser_expression_reference(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ExpressionReference
parser_expression_reference(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionReference expression = NULL;
    
    expression = parser_expression_functionCall(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created FunctionCallExpression.");
        goto END;
    }
    
    expression = parser_expression_variable(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created VariableExpression.");
        goto END;
    }
    
    expression = parser_expression_generate(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created GenerateExpression.");
        goto END;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
parser_expression_factor(
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
        ExpressionIntegerLiteral integerLiteral = Memory_malloc(sizeof(struct ExpressionIntegerLiteralTag));
        memset(integerLiteral, 0x00, sizeof(struct ExpressionIntegerLiteralTag));
        integerLiteral->value = string_toInteger(token->buffer);
        
        expression = expression_new(EXPRESSION_KIND_INTEGER_LITERAL);
        expression->of._integerLiteral_ = integerLiteral;
        
        parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check boolean literal.");
    if (token->type == TOKEN_TYPE_BOOLEAN_LITERAL)
    {
        Logger_dbg("This is an integer literal token.");
        ExpressionBooleanLiteral booleanLiteral = Memory_malloc(sizeof(struct ExpressionBooleanLiteralTag));
        memset(booleanLiteral, 0x00, sizeof(struct ExpressionBooleanLiteralTag));
        booleanLiteral->value = string_toBoolean(token->buffer);
        
        expression = expression_new(EXPRESSION_KIND_BOOLEAN_LITERAL);
        expression->of._booleanLiteral_ = booleanLiteral;
        
        parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check string literal.");
    if (token->type == TOKEN_TYPE_STRING_LITERAL)
    {
        Logger_dbg("This is a string literal token.");
        ExpressionStringLiteral stringLiteral = Memory_malloc(sizeof(struct ExpressionStringLiteralTag));
        memset(stringLiteral, 0x00, sizeof(struct ExpressionStringLiteralTag));
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
        expression = parser_expression(parser);
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
        ExpressionReference reference = parser_expression_reference(parser);
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
expression_parser_multiply_divide(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parser_expression_factor(parser);
    
    
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

    
    parser_next(parser);
    right = parser_expression(parser);
    
    
    Logger_dbg("Create MultiplyDivide Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
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
parser_expression_plus_minus(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = expression_parser_multiply_divide(parser);
    
    
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

    
    parser_next(parser);
    right = parser_expression(parser);
    
    
    Logger_dbg("Create PlusMinus Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
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
parser_expression_compare(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parser_expression_plus_minus(parser);
    
    token = parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_EQUAL)
    {
        kind = OPERATION_KIND_EQUAL;
    }
    else if (token->type == TOKEN_TYPE_NOT_EQUAL)
    {
        kind = OPERATION_KIND_NOT_EQUAL;
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

    
    parser_next(parser);
    right = parser_expression(parser);
    
    
    Logger_dbg("Create Compare Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
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
parser_expression_not(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = parser_expression_compare(parser);
    
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

    parser_next(parser);
    right = parser_expression(parser);
    
    Logger_dbg("Create NotEqual Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
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
parser_expression_and_or(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    
    left = parser_expression_compare(parser);
    
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

    
    parser_next(parser);
    right = parser_expression(parser);
    
    
    Logger_dbg("Create AndOr Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
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
parser_expression(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    
    expression = parser_expression_and_or(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Boolean
parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    Parser parser = parser_new(tokens);
    Boolean result = FALSE;
    Statement statement = NULL;
    DefinitionFunction functionDefinition = NULL;
    Token errorToken = NULL;
    
    Logger_dbg("Loop start.");
    parser_next(parser);
    while (TRUE)
    {
        functionDefinition = definition_function_parse(parser);
        if (functionDefinition != NULL)
        {
            Logger_dbg("Add created FunctionDefinition and parse next.");
            list_add(Interpreter_functions(interpreter), functionDefinition);
            continue;
        }
        
        Logger_dbg("Check statement.");
        statement = Statement_parse(parser);
        if (statement != NULL)
        {
            Logger_dbg("Add created statement and parse next.");
            list_add(Interpreter_statements(interpreter), statement);
            continue;
        }
        
        if (parser_is_end(parser) == TRUE)
            break;
        
        errorToken = parser_getCurrent(parser);
        parser_error(errorToken);
        goto END;
    }
    
    Logger_dbg("Loop end.");
    
    
    Logger_dbg("Parse Successed.");
    result = TRUE;
    
END:
	parser_dispose(parser);
    Logger_trc("[  END  ]%s", __func__);
    return result;
}


