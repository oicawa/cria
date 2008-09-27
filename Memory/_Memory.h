#ifndef PRIVATE_MEMORY_H_INCLUDED
#define PRIVATE_MEMORY_H_INCLUDED

#include "Memory.h"

typedef union HeaderTag Header;


struct MemoryTag {
    FILE                *error_fp;
    MemoryErrorHandler  error_handler;
    MemoryFailMode      fail_mode;
    Header              *block_header;
};

#endif /* PRIVATE_MEMORY_H_INCLUDED */

