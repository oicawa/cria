#ifndef PUBLIC_TOKENIZER_H_INCLUDED
#define PUBLIC_TOKENIZER_H_INCLUDED


#include "Cria.h"
#include "List.h"
#include "String.h"
#include "Boolean.h"
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


Token
Token_new(
    TokenType type,
    String value
);


/*
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



String
Token_get_file_path(
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
*/

#endif
