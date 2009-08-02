#ifndef PUBLIC_HASH_H_INCLUDED
#define PUBLIC_HASH_H_INCLUDED


#include "Cria.h"
#include "List.h"



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



int
Hash_get_count(
	Hash hash
);



List
Hash_get_keys(
	Hash hash
);



void*
Hash_get(
	Hash hash,
	char* key
);



#define Hash_log_key(hash) \
	do { \
		int i = 0; \
		Logger_dbg("Hash keys:%p", hash); \
		for (i = 0; i < List_count(Hash_get_keys(hash)); i++) \
		{ \
			String name = List_get(Hash_get_keys(hash), i); \
			Logger_dbg(" >> %s", name); \
		} \
	} while(0) \



#endif
