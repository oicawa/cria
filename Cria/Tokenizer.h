#ifndef PUBLIC_TOKENIZER_H_INCLUDED
#define PUBLIC_TOKENIZER_H_INCLUDED


#include "List.h"
#include "String.h"


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
typedef struct  TokenizerTag *Tokenizer;



List
Tokenizer_create_tokens(
    char*   filePath
);



TokenType
Token_type(
	Token token
);



int
Token_row(
	Token token
);



String
Token_buffer(
	Token token
);



int
Token_column(
	Token token
);



#define Token_log(__token__) \
	do { \
		Logger_dbg("token is [%p]", __token__); \
		char* __buffer__ = NULL; \
		if (__token__ == NULL) \
			Logger_dbg("Token type=NULL."); \
		else if (Token_buffer(__token__) != NULL) \
		{ \
			__buffer__ = Token_buffer(__token__); \
			Logger_dbg("Token type=%2d(%3d, %2d) [%s]", Token_type(__token__), Token_row(__token__), Token_column(__token__), __buffer__); \
		} \
	} while(0) \


#endif
