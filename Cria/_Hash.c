#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "String.h"

#include "_Hash.h"


Hash
Hash_new(
    int size
)
{
    Logger_trc("[ START ]%s", __func__);
    Hash hash = Memory_malloc(sizeof(struct HashTag));
    memset(hash, 0x00, sizeof(struct HashTag));
    hash->size = size;
    hash->count = 0;
    Entry* entrys = Memory_malloc(size);
    memset(entrys, 0x00, size);
    hash->entrys = entrys;
    
    Logger_trc("[  END  ]%s", __func__);
    return hash;
}



unsigned int 
Hash_get_key_value(
	int size,
    char* key
)
{
    int hash_value = 0;
    int c;
    
    while ((c = *key) != '\0')
    {
        hash_value += (int)c;
        key++;
    }
    
    return (hash_value % size);
}



Entry
Hash_get_entry(
	Hash hash,
	int hash_value,
	char* key
)
{
	Entry entry = NULL;
	
	return entry;
}



void
Hash_put(
	Hash hash,
	char* key,
	void* object
)
{
    Logger_trc("[ START ]%s", __func__);
    int hash_value;
    Entry entry;
    Entry cursor;
    
    
    hash_value = Hash_get_key_value(hash->size, key);


    entry = Hash_get_entry(hash, hash_value, key);
    if (entry != NULL)
		goto END;


    entry = Memory_malloc(sizeof(struct EntryTag));
    memset(entry, 0x00, sizeof(struct EntryTag));
    entry->key = String_clone(key);
    entry->hash_value = hash_value;
    entry->next = NULL;


	cursor = (Entry)((hash->entrys)[hash_value]);
	if (cursor == NULL)
	{
		(hash->entrys)[hash_value] = entry;
		goto END;
	}
	
	while (cursor->next != NULL)
		cursor = cursor->next;
	
	cursor->next = entry;

END:
	entry->object = object;
	
    Logger_trc("[  END  ]%s", __func__);
    return;
}



void*
Hash_get(
	Hash hash,
	char* key
)
{
	void* object = NULL;
	
	return object;
}



