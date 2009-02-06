#ifndef PRIVATE_PARSER_H_INCLUDED
#define PRIVATE_PARSER_H_INCLUDED



#include "List.h"

#include "Parser.h"



struct ParserTag
{
    Interpreter interpreter;
    List tokens;
    Item current;
    Item next;
    Item mark;
};



#endif
