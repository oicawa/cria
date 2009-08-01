#include "Cria.h"
#include "Memory.h"



void*
Memory_malloc(
    size_t  size
)
{
    void* pointer = NULL;
    pointer = GC_MALLOC(size);
    memset(pointer, 0x00, size);
    return pointer;
}



void
Memory_reset(
    void* pointer,
    size_t size
)
{
    memset(pointer, 0x00, size);
}
