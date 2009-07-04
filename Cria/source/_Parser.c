#include "Memory.h"
#include "Logger.h"

#include "Tokenizer.h"
#include "Definition.h"
#include "Statement.h"
#include "Reference.h"
#include "Expression.h"
#include "Loader.h"
#include "Interpreter.h"

#include "_Parser.h"


/*
void
Parser_errorFunction(
    Parser parser,
	Token token,
	char* file,
	int line
)
{
    String buffer = Token_buffer(token);
    String file_path = parser->path;
    int row = Token_row(token);
    int column = Token_column(token);
    
    Logger_err("Syntax error near '%s'. (file:%s, line:%d, column:%d) [%s, %d]\n", buffer, file_path, row, column, file, line);
    fprintf(stderr, "Syntax error near '%s'. (file:%s, line:%d, column:%d) [%s, %d]\n", buffer, file_path, row, column, file, line);
	exit(1);
}


Parser
Parser_new(
    Interpreter interpreter,
    List list,
    String path
)
{
    Logger_trc("[ START ]%s", __func__);
    Parser parser = Memory_malloc(sizeof(struct ParserTag));
    
    parser->interpreter = interpreter;
    parser->tokens = list;
    parser->current = NULL;
    parser->next = List_startItem(list);
    parser->mark = NULL;
    parser->path = String_new(path);
    
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
		Parser_error(parser, token);
	
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


void
Parser_insert(
    Parser parser,
    TokenType type
)
{
    Logger_trc("[ START ]%s", __func__);
    Token current = Parser_getCurrent (parser);
    Token token = Token_new(type, Token_row(current), Token_column(current), "<<NEW_LINE>>(dummy for block)");
    
    Item new_line = Item_new(token);
    List_insert_item(parser->tokens, parser->current, new_line);
    parser->next = parser->current;
    parser->current = new_line;
    
    Logger_trc("[  END  ]%s", __func__);
}




Boolean
Parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter,
    String path
)
{
    Logger_trc("[ START ]%s", __func__);
    Parser parser = Parser_new(interpreter, tokens, path);
    Boolean result = FALSE;
    Statement statement = NULL;
    DefinitionFunction functionDefinition = NULL;
    DefinitionClass classDefinition = NULL;
    Loader loader = NULL;
    Token errorToken = NULL;
    
    
    Logger_dbg("Loop start.");
    Parser_next(parser);
    
    
    while (TRUE)
    {
        loader = Loader_parse(parser);
        if (loader == NULL)
        {
            break;
        }
        
        Logger_dbg("Load library.");
        Loader_load(loader, interpreter);
    }
    
    
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
        Parser_error(parser, errorToken);
        goto END;
    }
    
    Logger_dbg("Loop end.");
    
    
    Logger_dbg("Parse Successed.");
    result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}
*/

