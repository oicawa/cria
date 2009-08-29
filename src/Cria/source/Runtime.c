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

 
#include "Cria.h"
#include "Interpreter.h"
#include "Logger.h"
#include "List.h"
#include "Reference.h"

#include "Runtime.h"

void
Runtime_error_write(
    Interpreter interpreter,
    const char* file_name,
    const char* func_name,
    int line,
    char*       format,
    ...
)
{
    va_list argp;
    va_start(argp, format);

    Logger_err("Runtime error. [%s, %d]\n", file_name, line);
    fprintf(stderr, "Runtime error. [%s, %d]\n", file_name, line);
    
    vfprintf(stderr, format, argp );
    fprintf(stderr, "\n");
    
    List stack = Interpreter_stack(interpreter);
    int count = List_count(stack);
    int i = 0;
    Reference reference = NULL;
    for (i = count - 1; 0 < i; i--)
    {
        reference = List_get(stack, i);
        String file_path = Reference_file_path(reference);
        int row = Reference_line(reference);
        int column = Reference_column(reference);
        fprintf(stderr, "[%s] line: %d, column: %d\n", file_path, row, column); 
    }
    
    exit(1);
}

