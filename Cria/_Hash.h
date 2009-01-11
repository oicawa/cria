#ifndef PRIVATE_HASH_H_INCLUDED
#define PRIVATE_HASH_H_INCLUDED


#include "Hash.h"



struct HashTag
{
	int size;
    int count;
    List keys;
    Entry* entrys;
};



struct EntryTag
{
    unsigned int hash_value;
    char* key;
    void* object;
    Entry next;
};



#endif
