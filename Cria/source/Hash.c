#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "String.h"
#include "Hash.h"


Hash
Hash_new(
    int size
)
{
    Logger_trc("[ START ]%s", __func__);
    Hash hash = Memory_malloc(sizeof(struct HashTag));
    hash->size = size;
    hash->count = 0;
    hash->keys = List_new();
    Entry* entrys = Memory_malloc(sizeof(Entry) * size);
    hash->entrys = entrys;
    
    Logger_trc("[  END  ]%s", __func__);
    return hash;
}



int
Hash_get_count(
	Hash hash
)
{
    return hash->count;
}



List
Hash_get_keys(
	Hash hash
)
{
    return hash->keys;
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
	unsigned int hash_value,
	char* key
)
{
    Logger_trc("[ START ]%s(hash is %p, hash_value='%d', key='%s')", __func__, hash, hash_value, key);
	Entry entry = NULL;
	//Entry* root = NULL;
	
	//root = hash->entrys;
	//int i = 0;
	//for (i = 0; i <= hash->size; i++)
	//{
		//Logger_dbg("--- i = %d --", i);
		//Logger_dbg("(root + %d) is %p", i, root + i);
		//Logger_dbg("*(root + %d) is %p", i, *(root + i));
	//}
	
	entry = (Entry)((hash->entrys)[hash_value]);
	Logger_dbg("entry is %p", entry);
	if (entry == NULL)
		goto END;
	
	Logger_dbg("entry->key = '%s'", entry->key);
	while (strcmp(entry->key, key) != 0)
	{
		Logger_dbg("entry->next is %p", entry->next);
		if (entry->next == NULL)
		{
			entry = NULL;
			break;
		}
		entry = entry->next;
	}

	Logger_dbg("entry is %p", entry);
END:	
    Logger_trc("[  END  ]%s", __func__);
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
    unsigned int hash_value;
    Entry entry;
    Entry cursor;
    
    
    hash_value = Hash_get_key_value(hash->size, key);


    entry = Hash_get_entry(hash, hash_value, key);
    if (entry != NULL)
		goto END;


    entry = Memory_malloc(sizeof(struct EntryTag));
    entry->key = String_new(key);
    entry->hash_value = hash_value;
    entry->next = NULL;


	cursor = (Entry)((hash->entrys)[hash_value]);
	if (cursor == NULL)
	{
		(hash->entrys)[hash_value] = entry;
		List_add(hash->keys, key);
		goto END;
	}
	
	while (cursor->next != NULL)
		cursor = cursor->next;
	
	cursor->next = entry;
    List_add(hash->keys, key);

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
    Logger_trc("[ START ]%s", __func__);
	void* object = NULL;
	Entry entry = NULL;
	
	unsigned int hash_value = Hash_get_key_value(hash->size, key);
	
	entry = Hash_get_entry(hash, hash_value, key);
	if (entry == NULL)
		goto END;
	
	object = entry->object;
	
END:
    Logger_trc("[  END  ]%s", __func__);
	return object;
}

