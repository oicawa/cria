#ifndef PUBLIC_INTERPRETER_H_INCLUDED
#define PUBLIC_INTERPRETER_H_INCLUDED

#include "Boolean.h"
#include "List.h"

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



int
Interpreter_row(
    Interpreter interpreter
);



List
Interpreter_functions(
	Interpreter interpreter
);



List
Interpreter_statements(
	Interpreter interpreter
);



void
Interpreter_setRow(
	Interpreter interpreter,
	int row
);



List
Interpreter_variables(
	Interpreter interpreter
);



List
Interpreter_classes(
	Interpreter interpreter
);



#endif
