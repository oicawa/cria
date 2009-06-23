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



#define Tokenizer_error(buffer, row, column, path) \
    do { \
        fprintf(stderr, "Syntax error near '%s' (file:[%s], line:%d, column:%d). [%s, %d]\n", buffer, path, row, column, __FILE__, __LINE__); \
        Logger_err("Token error near '%s' in file '%s'. (line:%d, column:%d) [%s, %d]\n", buffer, path, row, column, __FILE__, __LINE__); \
        exit(1); \
    } while(0) \



#endif
