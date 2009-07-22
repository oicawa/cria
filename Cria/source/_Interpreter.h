#ifndef PRIVATE_INTERPRETER_H_INCLUDED
#define PRIVATE_INTERPRETER_H_INCLUDED



#include "Cria.h"
#include "List.h"
#include "Hash.h"

#include "Interpreter.h"

struct InterpreterTag
{
    List statements;
    Hash libraries;
    Hash variables;
    Hash functions;
    Hash classes;
    int row;
    int column;
    int indentLevel;
    Hash loaded_files;
    List stack;
};



#endif
