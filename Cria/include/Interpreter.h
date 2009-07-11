#ifndef PUBLIC_INTERPRETER_H_INCLUDED
#define PUBLIC_INTERPRETER_H_INCLUDED

#include "Boolean.h"
#include "List.h"
#include "String.h"
#include "Hash.h"



typedef struct InterpreterTag *Interpreter;


/*
List
Interpreter_stack(
    Interpreter interpreter
);



void
Interpreter_stack_add(
    Interpreter interpreter,
    void* expression
);



void
Interpreter_stack_pop(
    Interpreter interpreter
);
*/


Interpreter
Interpreter_new(
    void
);


/*
Boolean
Interpreter_compile(
    Interpreter interpreter,
    char*       filePath
);
*/


void
Interpreter_run(
    Interpreter interpreter
);


/*
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
Interpreter_get_imports(
    Interpreter interpreter
);
*/


void
Interpreter_dispose(
	Interpreter interpreter
);


/*
Boolean
Interpreter_has_loaded(
    Interpreter interpreter,
    String file_path
);



void
Interpreter_add_loaded_file(
    Interpreter interpreter,
    String file_path
);
*/


#endif
