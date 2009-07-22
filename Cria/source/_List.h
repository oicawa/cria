#ifndef PRIVATE_LIST_H_INCLUDED
#define PRIVATE_LIST_H_INCLUDED



#include "Cria.h"
#include "List.h"



struct ItemTag {
    void *object;
    Item prev;
    Item next;
};



struct ListTag {
    Item item;
    Item last;
    int count;
};



#endif
