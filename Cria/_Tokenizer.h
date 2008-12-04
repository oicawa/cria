#ifndef PRIVATE_TOKENIZER_H_INCLUDED
#define PRIVATE_TOKENIZER_H_INCLUDED


#include <stdio.h>

#include "_String.h"
#include "_List.h"


typedef enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_CONSTANT,
    TOKEN_TYPE_CLASS_LITERAL,
    TOKEN_TYPE_INDENT,
    TOKEN_TYPE_DEDENT,
    TOKEN_TYPE_INTEGER_LITERAL,
    TOKEN_TYPE_BOOLEAN_LITERAL,
    TOKEN_TYPE_STRING_LITERAL,
    TOKEN_TYPE_IF,
    TOKEN_TYPE_FOR,
    TOKEN_TYPE_IN,
    TOKEN_TYPE_ELIF,
    TOKEN_TYPE_ELSE,
    TOKEN_TYPE_WHILE,
    TOKEN_TYPE_THROW,
    TOKEN_TYPE_RETURN,
    TOKEN_TYPE_RETURN_VALUE,
    TOKEN_TYPE_BREAK,
    TOKEN_TYPE_CONTINUE,
    TOKEN_TYPE_GOTO,
    TOKEN_TYPE_CATCH,
    TOKEN_TYPE_FINALLY,
    TOKEN_TYPE_SUBSTITUTE,
    TOKEN_TYPE_INCREMENT,
    TOKEN_TYPE_DECREMENT,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_MULTIPLY,
    TOKEN_TYPE_DEVIDE,
    TOKEN_TYPE_MODULO,
    TOKEN_TYPE_PARENTHESIS_LEFT,
    TOKEN_TYPE_PARENTHESIS_RIGHT,
    TOKEN_TYPE_BRACKET_LEFT,
    TOKEN_TYPE_BRACKET_RIGHT,
    TOKEN_TYPE_BRACE_LEFT,
    TOKEN_TYPE_BRACE_RIGHT,
    TOKEN_TYPE_GENERICS_LEFT,
    TOKEN_TYPE_GENERICS_RIGHT,
    TOKEN_TYPE_LESS_THAN,
    TOKEN_TYPE_LESS_EQUAL,
    TOKEN_TYPE_EQUAL,
    TOKEN_TYPE_NOT_EQUAL,
    TOKEN_TYPE_OR,
    TOKEN_TYPE_AND,
    TOKEN_TYPE_GREATER_EQUAL,
    TOKEN_TYPE_GREATER_THAN,
    TOKEN_TYPE_EXTENDS,
    TOKEN_TYPE_COLON,
    TOKEN_TYPE_MEMBER,
    TOKEN_TYPE_PERIOD,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_UNDER,
    TOKEN_TYPE_NULL,
    TOKEN_TYPE_MONADIC_MINUS,
    TOKEN_TYPE_NEW_LINE,
    TOKEN_TYPE_DUMMY,
} TokenType;



typedef struct  TokenTag *Token;
struct TokenTag
{
    TokenType   type;
    int         row;
    int         column;
    String      buffer;
};



typedef struct  TokenizerTag *Tokenizer;
struct TokenizerTag
{
    FILE            *file;
    int             row;
    int             column;
    char            next;
    int             indentLevel;
};



List
tokenizer_create_tokens(
    char*   filePath
);



#define tokenizer_error(buffer, row, column) \
    do { \
        fprintf(stderr, "Syntax error near '%s'. [%s, %d]\n", buffer, __FILE__, __LINE__); \
        Logger_err("Token error near '%s'. (line:%d, column:%d) [%s, %d]\n", buffer, row, column, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#define token_log(token) \
    do \
    { \
    	Logger_dbg("token is [%p]", token); \
        char* buffer = NULL; \
        if (token == NULL) \
            Logger_dbg("Token type=NULL."); \
        else if (token->buffer != NULL) \
        { \
            buffer = token->buffer->pointer; \
            Logger_dbg("Token type=%2d(%3d, %2d) [%s]", token->type, token->row, token->column, buffer); \
        } \
    } while(0) \



#endif
