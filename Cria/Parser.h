#ifndef PUBLIC_PARSER_H_INCLUDED
#define PUBLIC_PARSER_H_INCLUDED


#include "Boolean.h"
#include "List.h"
#include "Tokenizer.h"
#include "Interpreter.h"


typedef struct  ParserTag *Parser;



Token
parser_getCurrent(
    Parser  parser
);



Item
parser_getPosition(
    Parser  parser
);



void
parser_setPosition(
    Parser  parser,
    Item    position
);



Boolean
parser_next(
    Parser  parser
);



Boolean
parser_eat(
	Parser parser,
	TokenType type,
	Boolean isNessesally
);



Boolean
parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter
);



void
parser_errorFunction(
	Token token,
	char* file,
	int line
);
#define parser_error(token) \
    parser_errorFunction(token, __FILE__, __LINE__)



#endif
