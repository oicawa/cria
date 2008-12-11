#ifndef PUBLIC_PARSER_H_INCLUDED
#define PUBLIC_PARSER_H_INCLUDED


#include "Boolean.h"
#include "List.h"
#include "Tokenizer.h"
#include "Interpreter.h"


typedef struct  ParserTag *Parser;



Token
Parser_getCurrent(
    Parser  parser
);



Item
Parser_getPosition(
    Parser  parser
);



void
Parser_setPosition(
    Parser  parser,
    Item    position
);



Boolean
Parser_next(
    Parser  parser
);



Boolean
Parser_eat(
	Parser parser,
	TokenType type,
	Boolean isNessesally
);



Boolean
Parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter
);



void
Parser_errorFunction(
	Token token,
	char* file,
	int line
);
#define Parser_error(token) \
    Parser_errorFunction(token, __FILE__, __LINE__)



#endif
