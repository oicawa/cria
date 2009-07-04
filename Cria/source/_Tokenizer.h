#ifndef PRIVATE_TOKENIZER_H_INCLUDED
#define PRIVATE_TOKENIZER_H_INCLUDED


#include "String.h"
#include "List.h"

#include "Tokenizer.h"


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
