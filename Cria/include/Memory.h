#ifndef PUBLIC_MEMORY_H_INCLUDED
#define PUBLIC_MEMORY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "Cria.h"



void
*Memory_malloc(
    size_t  size
);



void
Memory_reset(
    void* pointer,
    size_t size
);



#endif  /* PUBLIC_MEMORY_H_INCLUDED */
