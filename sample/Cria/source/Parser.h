#ifndef PUBLIC_PARSER_H_INCLUDED
#define PUBLIC_PARSER_H_INCLUDED


#include "Cria.h"
#include "List.h"
#include "Tokenizer.h"
#include "Interpreter.h"



struct ParserTag
{
    Interpreter interpreter;
    List tokens;
    Item current;
    Item next;
    Item mark;
    String path;
};



Interpreter
Parser_getInterpreter(
    Parser parser
);



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
    Interpreter interpreter,
    String path
);



void
Parser_errorFunction(
    Parser parser,
	Token token,
	char* file,
	int line
);
#define Parser_error(parser, token) \
    Parser_errorFunction(parser, token, __FILE__, __LINE__)



void
Parser_insert(
    Parser parser,
    TokenType type
);



#endif