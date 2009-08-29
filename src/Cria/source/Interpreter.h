/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
#ifndef PUBLIC_INTERPRETER_H_INCLUDED
#define PUBLIC_INTERPRETER_H_INCLUDED

#include "Cria.h"
#include "List.h"
#include "String.h"
#include "Hash.h"



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



#endif
