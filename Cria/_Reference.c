#include "_Cria.h"



Reference
reference_new(
    ReferenceType   type
)
{
    Reference reference = NULL;
    
    reference = Memory_malloc(sizeof(struct ReferenceTag));
    memset(reference, 0x00, sizeof(struct ReferenceTag));
    reference->type = type;
    
    return reference;
}



