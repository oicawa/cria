#ifndef PRIVATE_INTERPRETER_H_INCLUDED
#define PRIVATE_INTERPRETER_H_INCLUDED



#include "List.h"
#include "Hash.h"

struct InterpreterTag
{
    List statements;
    Hash imports;
    Hash variables;
    Hash functions;
    Hash classes;
    int row;
    int column;
    int indentLevel;
};



#endif
