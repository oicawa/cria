#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "gc.h"
#include "_Memory.h"



void*
Memory_malloc(
    size_t  size
)
{
    return GC_MALLOC(size);
}


