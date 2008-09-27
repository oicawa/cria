#ifndef PUBLIC_MEMORY_H_INCLUDED
#define PUBLIC_MEMORY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>



typedef enum {
    MEMORY_FAIL_MODE_EXIT,
    MEMORY_FAIL_MODE_RETURN
} MemoryFailMode;



typedef struct MemoryTag *Memory;
typedef void (*MemoryErrorHandler)(Memory, char *, int, char *);
typedef struct MemoryStorageTag *MemoryStorage;



extern Memory mem_default_controller;



/*
 * Don't use mem_*_func function.
 * There are private functions of MEM module.
 */
Memory
Memory_new(
    void
);


void
Memory_dispose(
    void
);


void
*Memory_mallocFunction(
    Memory  controller,
    char    *filename, 
    int     line, 
    size_t  size
);
#define Memory_malloc(size)\
    (Memory_mallocFunction(mem_default_controller, __FILE__, __LINE__, size))
#define Memory_mallocAt(file, line, size)\
    (Memory_mallocFunction(mem_default_controller, file, line, size))


void
*Memory_reallocFunction(
    Memory  controller,
    char    *filename, 
    int     line, 
    void    *ptr, 
    size_t  size
);
#define Memory_realloc(ptr, size)\
    (Memory_reallocFunction(mem_default_controller, __FILE__, __LINE__, ptr, size))



char
*Memory_stringFunction(
    Memory  controller,
    char    *filename, 
    int     line, 
    char    *str
);
#define Memory_string(str)\
    (Memory_stringFunction(mem_default_controller, __FILE__, __LINE__, str))



MemoryStorage
Memory_openStorageFunction(
    Memory  controller,
    char    *filename, 
    int     line, 
    int     page_size
);
#define Memory_openStorage(page_size)\
    (Memory_openStorageFunction(mem_default_controller, __FILE__, __LINE__, page_size))



void
*Memory_mallocStorageFunction(
    Memory          controller,
    char            *filename, 
    int             line,
    MemoryStorage   storage,
    size_t          size
);
#define Memory_mallocStorage(storage, size)\
    (Memory_mallocStorageFunction(mem_default_controller, __FILE__, __LINE__, storage, size))



void
Memory_freeFunction(
    Memory  controller,
    void    *ptr
);
#define Memory_free(ptr)\
    (Memory_freeFunction(mem_default_controller, ptr))



void
Memory_disposeStorageFunction(
    Memory          controller,
    MemoryStorage   storage
);
#define Memory_disposeStorage(storage)\
    (Memory_disposeStorageFunction(mem_default_controller, storage))



void
Memory_setErrorHandler(
    Memory              controller,
    MemoryErrorHandler  handler
);



void
Memory_setFailMode(
    Memory          controller,
    MemoryFailMode  mode
);



void
Memory_dumpBlocksFunction(
    Memory  controller, 
    FILE    *fp
);
#define Memory_dumpBlocks(fp)\
    (Memory_dumpBlocksFunction(mem_default_controller, fp))



void
Memory_checkBlockFunction(
    Memory  controller,
    char    *filename,
    int     line, 
    void    *p
);
#define Memory_checkBlock(p)\
    (Memory_checkBlockFunction(mem_default_controller, __FILE__, __LINE__, p))



void
Memory_checkAllBlocksFunction(
    Memory  controller,
    char    *filename,
    int     line
);
#define Memory_checkAllBlocks()\
    (Memory_checkAllBlocksFunction(mem_default_controller, __FILE__, __LINE__))



#endif  /* PUBLIC_MEMORY_H_INCLUDED */
