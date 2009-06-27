#ifndef PUBLIC_RUNTIME_H_INCLUDED
#define PUBLIC_RUNTIME_H_INCLUDED



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
