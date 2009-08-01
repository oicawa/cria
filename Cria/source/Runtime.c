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

