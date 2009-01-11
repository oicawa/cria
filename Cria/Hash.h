#ifndef PUBLIC_HASH_H_INCLUDED
#define PUBLIC_HASH_H_INCLUDED


#include "List.h"



typedef struct EntryTag *Entry;
typedef struct HashTag *Hash;



Hash
Hash_new(
	int size
);



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



void
Hash_put(
	Hash hash,
	char* key,
	void* object
);


#endif
