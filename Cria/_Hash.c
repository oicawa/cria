#include "_Cria.h"

typedef HashEntryTag    *Entry;
typedef HashTypeTag     *HashType;
typedef HashTag         *Hash;

struct HashTag
{
    HashType    type;
    int         binsCount;
    int         entriesCount;
    HashEntry*  bins;
};


struct HashTypeTag
{
    int (*compare)();   //比較関数
    int (*hash)();      //ハッシュ関数
};


struct HashEntryTag
{
    unsigned int    value;
    string          key;
    string          record;
    HashEntry       next;
};


Hash
hash_new(
    int         size,
    HashType    type;
)
{
    Logger_trc("[ START ]%s", __func__);
    Hash hash = Memory_malloc(sizeof(struct HashTag));
    memset(hash, 0x00, sizeof(struct HashTag));
    hash->type = type;
    hash->binsCount = size;
    hash->entriesCount = 0;
    hash->bins = NULL;
    
    Logger_trc("[  END  ]%s", __func__);
    return hash;
}



static int
numcmp(
    long x,
    long y
)
{
    return x != y;
}

static int
numhash(
    long n
)
{
    return n;
}



#define do_hash(hash, key) \
    (unsigned int)(*(hash)->type->value)((key))



#define FIND_ENTRY(hash, entry, value, position) \
    do {\
        position = hash_val%(hash)->binsCount;\
        entry = (hash)->bins[position];\
        if (PTR_NOT_EQUAL(hash, entry, value, key)) \
        {\
            while (PTR_NOT_EQUAL(hash, entry->next, value, key)) \
            {\
                entry = entry->next;\
            }\
            entry = entry->next;\
        }\
    } while (0)



#define PTR_NOT_EQUAL(hash, entry, hashValue, key) \
    ((entry) != 0 && (entry->value != (hashValue) || !EQUAL((hash), (key), (entry)->key)))



#define EQUAL(table,x,y) \
    ((x)==(y) || (*table->type->compare)((x),(y)) == 0)



int
hash_lookup(
    Hash            hash,
    register char   *key,
    char            **value
)
{
    unsigned int hash_val;
    unsigned int bin_pos;
    register HashEntry  entry;
    
    hash_val = do_hash(key, hash);
    FIND_ENTRY(hash, entry, hash_val, bin_pos);

    if (entry == 0)
    {
        return 0;
    }
    else
    {
        if (value != 0)  
            *value = entry->record;
        return 1;
    }
}







