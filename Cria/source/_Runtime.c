#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>

#include "Interpreter.h"
#include "Logger.h"

#include "_Runtime.h"

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

    Logger_err("Runtime error. (line:%d) [%s, %d]\n", Interpreter_row(interpreter), file_name, line);
    fprintf(stderr, "Runtime error. (line:%d) [%s, %d]\n", Interpreter_row(interpreter), file_name, line);
    
    //fprintf(stderr, format, level, file_name, line, func_name);
    vfprintf(stderr, format, argp );
    fprintf(stderr, "\n");
    
    exit(1);
}
