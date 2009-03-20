#ifndef PUBLIC_INTERPRETER_H_INCLUDED
#define PUBLIC_INTERPRETER_H_INCLUDED

#include "Boolean.h"
#include "List.h"
#include "Hash.h"

typedef struct InterpreterTag *Interpreter;



Interpreter
Interpreter_new(
    void
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



Hash
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



Hash
Interpreter_variables(
	Interpreter interpreter
);



Hash
Interpreter_classes(
	Interpreter interpreter
);



Hash
Interpreter_imports(
    Interpreter interpreter
);



#endif
