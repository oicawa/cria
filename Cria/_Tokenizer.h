#ifndef PRIVATE_TOKENIZER_H_INCLUDED
#define PRIVATE_TOKENIZER_H_INCLUDED


#include "String.h"
#include "List.h"

#include "Tokenizer.h"

struct TokenTag
{
    TokenType   type;
    int         row;
    int         column;
    String      buffer;
};



struct TokenizerTag
{
    FILE            *file;
    int             row;
    int             column;
    char            next;
    int             indentLevel;
};



#define tokenizer_error(buffer, row, column) \
    do { \
        fprintf(stderr, "Syntax error near '%s'. [%s, %d]\n", buffer, __FILE__, __LINE__); \
        Logger_err("Token error near '%s'. (line:%d, column:%d) [%s, %d]\n", buffer, row, column, __FILE__, __LINE__); \
        exit(1); \
    } while(0) \



#endif
