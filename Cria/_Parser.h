#ifndef PRIVATE_PARSER_H_INCLUDED
#define PRIVATE_PARSER_H_INCLUDED



#include "List.h"



struct ParserTag
{
    List    tokens;
    Item    current;
    Item    next;
    Item    mark;
};



#endif
