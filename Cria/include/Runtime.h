#ifndef PUBLIC_RUNTIME_H_INCLUDED
#define PUBLIC_RUNTIME_H_INCLUDED

/*
#define Runtime_error(interpreter, message) \
    do { \
        Logger_err("Runtime error. (line:%d) [%s, %d]\n", Interpreter_row(interpreter), __FILE__, __LINE__); \
        fprintf(stderr, "Runtime error. (line:%d) [%s, %d]\n%s", Interpreter_row(interpreter), __FILE__, __LINE__, message); \
        exit(1); \
    } while(0) \
//*/

/*
#define Logger_err(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_ERROR, __VA_ARGS__))
*/


void
Runtime_error_write(
    Interpreter interpreter,
    const char* file_name,
    const char* func_name,
    int line,
    char*       format,
    ...
);



#define Runtime_error(interpreter, ...) \
    Runtime_error_write(interpreter, __FILE__, __func__, __LINE__, __VA_ARGS__);




#endif
