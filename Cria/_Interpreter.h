#ifndef PRIVATE_INTERPRETER_H_INCLUDED
#define PRIVATE_INTERPRETER_H_INCLUDED



#include "List.h"

struct InterpreterTag
{
    List    statements;
    List    variables;
    List    functions;
    List    classes;
    int     row;
    int     column;
    int     indentLevel;
};



#endif
