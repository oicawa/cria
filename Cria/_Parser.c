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
    Token_log(((Token)(current->object)));
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
        Token_log(token);
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
ExpressionParameters_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionParameters parameters = NULL;
    Expression expression = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    List list = List_new();
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    Logger_dbg("Loop start.");
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Parse expression.");
        expression = Expression_parse(parser);
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
        Token_log(token);
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
            Token_log(token);
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
ReferenceVariable_parse(
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
    	Token_log(token);
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
    	Token_log(token);
    	Logger_dbg("Not '.' and ' = '");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    memset(variable, 0x00, sizeof(struct ReferenceVariableTag));
    variable->name = string_clone(name);
	Logger_dbg("Created ReferenceVariable");

    
    reference = Reference_new(REFERENCE_TYPE_VARIABLE);
    reference->of.variable = variable;
    
    
    if (token->type != TOKEN_TYPE_PERIOD)
    {
    	Token_log(token);
    	Logger_dbg("Not '.'");
        goto END;
    }
    
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    Token_log(token);
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        parser_error(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
ReferenceFunctionCall_parse(
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
    	Token_log(token);
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
    	Token_log(token);
	    Logger_dbg("Not '('.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    parser_next(parser);
    parameters = ExpressionParameters_parse(parser);
    if (parameters == NULL)
    {
	    Logger_dbg("Not parameters.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
    	Token_log(token);
	    Logger_dbg("Not ')'.");
    	parser_error(token);
        goto END;
    }
    
    functionCall = Memory_malloc(sizeof(struct ReferenceFunctionCallTag));
    memset(functionCall, 0x00, sizeof(struct ReferenceFunctionCallTag));
    functionCall->name = string_clone(name);
    functionCall->parameters = parameters;
    Logger_dbg("Created ReferenceFunctionCall");

    reference = Reference_new(REFERENCE_TYPE_FUNCTION_CALL);
    reference->of.function = functionCall;
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD)
    {
    	Token_log(token);
	    Logger_dbg("Not '.'.");
    	goto END;
    }
    
	parser_next(parser);
	reference->next = Reference_parse(parser);
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
Reference_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    Reference reference = NULL;
    
    
    reference = ReferenceVariable_parse(parser);
    if (reference != NULL)
        goto END;
    
    reference = ReferenceFunctionCall_parse(parser);
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


