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



unsigned int 
hash_getKey(
    char *s
)
{
    unsigned int hash = 0;
    for (;*s != '\0'; s++)
    {
        hash = hash * 137 + *s;
    }
    
    return h % 1987;
}



