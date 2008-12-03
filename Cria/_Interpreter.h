#ifndef PRIVATE_INTERPRETER_H_INCLUDED
#define PRIVATE_INTERPRETER_H_INCLUDED



#include "_Boolean.h"
#include "_List.h"

typedef struct InterpreterTag *Interpreter;
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



Interpreter
Interpreter_new(
    void
);



void
Interpreter_dispose(
    Interpreter interpreter
);



Boolean
Interpreter_compile(
    Interpreter interpreter,
    char*       filePath
);



void
Interpreter_run(
    Interpreter interpreter
);



#endif
