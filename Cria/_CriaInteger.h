#ifndef PRIVATE_CRIAINTEGER_H_INCLUDED
#define PRIVATE_CRIAINTEGER_H_INCLUDED


#include "_Boolean.h"
#include "_CriaId.h"

typedef struct CriaIntegerTag
{
    struct CriaIdTag    id;
    Boolean             isLiteral;
    int                 value;
} *CriaInteger;





#endif