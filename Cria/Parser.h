#ifndef PUBLIC_PARSER_H_INCLUDED
#define PUBLIC_PARSER_H_INCLUDED


#include "Boolean.h"
#include "List.h"
#include "Interpreter.h"


typedef struct  ParserTag *Parser;



Boolean
parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter
);



#define parser_error(token) \
    do { \
        Logger_err("Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", (token)->buffer->pointer, (token)->row, (token)->column, __FILE__, __LINE__); \
        fprintf(stderr, "Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", (token)->buffer->pointer, (token)->row, (token)->column, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#endif
