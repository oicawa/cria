#ifndef PRIVATE_TOKENIZER_H_INCLUDED
#define PRIVATE_TOKENIZER_H_INCLUDED


#include "String.h"
#include "List.h"

#include "Tokenizer.h"

/*
struct ReservedWordTag
{
    TokenType type;
    String value;
}


static struct ReservedWordTag RESERVED_WORDS[] =
{
    { "null", TOKEN_TYPE_NULL },
    { "block", TOKEN_TYPE_BLOCK }
};
*/

struct TokenizerTag
{
    String          path;
    FILE            *file;
    int             row;
    int             column;
    char            next;
    int             indentLevel;
};



#endif
