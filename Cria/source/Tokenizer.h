#ifndef PUBLIC_TOKENIZER_H_INCLUDED
#define PUBLIC_TOKENIZER_H_INCLUDED


#include "Cria.h"
#include "List.h"
#include "String.h"
#include "Interpreter.h"


struct TokenTag
{
    TokenType type;
    String value;
    int row;
    int column;
    String file_path;
};

struct TokenCheckerTag
{
    TokenType type;
    String value;
    Boolean terminate;
};

List
Tokenizer_create_tokens(
    char* filePath,
    Boolean is_main
);

/*
Token
Token_new(
    TokenType type,
    String value
);
*/

#endif
