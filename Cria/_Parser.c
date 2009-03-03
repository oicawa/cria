#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "Tokenizer.h"
#include "Definition.h"
#include "Statement.h"
#include "Reference.h"
#include "Expression.h"

#include "_Parser.h"





void
Parser_errorFunction(
	Token token,
	char* file,
	int line
)
{
	Logger_err("Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", Token_buffer(token), Token_row(token), Token_column(token), file, line);
	fprintf(stderr, "Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", Token_buffer(token), Token_row(token), Token_column(token), file, line);
	exit(1);
}


Parser
Parser_new(
    Interpreter interpreter,
    List list
)
{
    Logger_trc("[ START ]%s", __func__);
    Parser parser = Memory_malloc(sizeof(struct ParserTag));
    memset(parser, 0x00, sizeof(struct ParserTag));
    
    parser->interpreter = interpreter;
    parser->tokens = list;
    parser->current = NULL;
    parser->next = List_startItem(list);
    parser->mark = NULL;
    
    Logger_trc("[  END  ]%s", __func__);
    return parser;
}



Interpreter
Parser_getInterpreter(
    Parser parser
)
{
    return parser->interpreter;
}



Token
Parser_getCurrent(
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
    Token token = (Token)(Item_getObject(item));
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
Parser_next(
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
    Token token = (Token)Item_getObject(current);
    Token_log(token);
    parser->current = current;
    parser->next = Item_getNext(current);
    
    return TRUE;
}



Item
Parser_getPosition(
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
Parser_setPosition(
    Parser  parser,
    Item    position
)
{
    Logger_trc("[ START ]%s", __func__);
    if (parser == NULL)
        return;
    
    parser->current = position;
    
    if (position == NULL)
        parser->next = List_startItem(parser->tokens);
    else
        parser->next = Item_getNext(position);
    
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
Parser_eat(
	Parser parser,
	TokenType type,
	Boolean isNessesally
)
{
	Boolean result = FALSE;
	Token token = Parser_getCurrent(parser);
	if (token == NULL)
		goto UNMATCH;
	
	if (Token_type(token) != type)
		goto UNMATCH;
	
	result = TRUE;
	Parser_next(parser);
	goto END;

UNMATCH:
	if (isNessesally == TRUE)
		Parser_error(token);
	
END:
	return result;
}



Boolean
Parser_is_end(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = NULL;
    Boolean result = FALSE;
    
    while (TRUE)
    {
        token = Parser_getCurrent(parser);
        Token_log(token);
        if (token == NULL)
        {
            result = TRUE;
            goto END;
        }
        
        if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
            goto END;
        
        if (Parser_next(parser) == FALSE)
        {
            result = TRUE;
            break;
        }
    }
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
Parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    Parser parser = Parser_new(interpreter, tokens);
    Boolean result = FALSE;
    Statement statement = NULL;
    DefinitionFunction functionDefinition = NULL;
    DefinitionClass classDefinition = NULL;
    Token errorToken = NULL;
    
    Logger_dbg("Loop start.");
    Parser_next(parser);
    while (TRUE)
    {
        classDefinition = DefinitionClass_parse(parser);
        if (classDefinition != NULL)
        {
            Logger_dbg("Add created DefinitionClass and parse next.");
            Hash_put(Interpreter_classes(interpreter), DefinitionClass_getName(classDefinition), classDefinition);
            continue;
        }
        
        functionDefinition = DefinitionFunction_parse(parser);
        if (functionDefinition != NULL)
        {
            Logger_dbg("Add created FunctionDefinition and parse next.");
            Hash_put(Interpreter_functions(interpreter), DefinitionFunction_get_name(functionDefinition), functionDefinition);
            continue;
        }
        
        Logger_dbg("Check statement.");
        statement = Statement_parse(parser);
        if (statement != NULL)
        {
            Logger_dbg("Add created statement and parse next.");
            List_add(Interpreter_statements(interpreter), statement);
            continue;
        }
        
        if (Parser_is_end(parser) == TRUE)
            break;
        
        errorToken = Parser_getCurrent(parser);
        Parser_error(errorToken);
        goto END;
    }
    
    Logger_dbg("Loop end.");
    
    
    Logger_dbg("Parse Successed.");
    result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}


