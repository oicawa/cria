#ifndef PRIVATE_PARSER_H_INCLUDED
#define PRIVATE_PARSER_H_INCLUDED



#include "_Boolean.h"
#include "_List.h"
#include "_Interpreter.h"



typedef struct  ParserTag *Parser;
struct ParserTag
{
    List    tokens;
    Item    current;
    Item    next;
    Item    mark;
};



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
