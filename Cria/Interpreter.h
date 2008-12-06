#ifndef PUBLIC_INTERPRETER_H_INCLUDED
#define PUBLIC_INTERPRETER_H_INCLUDED

#include "Boolean.h"

typedef struct InterpreterTag *Interpreter;



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
