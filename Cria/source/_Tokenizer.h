#ifndef PRIVATE_TOKENIZER_H_INCLUDED
#define PRIVATE_TOKENIZER_H_INCLUDED


#include "String.h"
#include "List.h"

#include "Tokenizer.h"

struct TokenTag
{
    TokenType type;
    int row;
    int column;
    String buffer;
    String file_path;
};



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
