#ifndef PRIVATE_RUNTIME_H_INCLUDED
#define PRIVATE_RUNTIME_H_INCLUDED


#define runtime_error(interpreter) \
    do { \
        Logger_err("Runtime error. (line:%d) [%s, %d]\n", (interpreter)->row, __FILE__, __LINE__); \
        fprintf(stderr, "Runtime error. (line:%d) [%s, %d]\n", (interpreter)->row, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#endif
