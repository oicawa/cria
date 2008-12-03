#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_List.h"



Item
item_newFunction(
    void    *object,
    char    *fileName,
    int     line
)
{
    Item item = Memory_mallocAt(fileName, line, sizeof(struct ItemTag));
    memset(item, 0x00, sizeof(struct ItemTag));
    item->object = object;
    item->next = NULL;
    return item;
}



void
item_dispose(
    Item    item
)
{
    Logger_cor("[ START ]%s", __func__);
    /*
    if (item == NULL)
    {
        Logger_cor("item is NULL.");
        goto END;
    }
    
    Logger_cor("Check item->next. (%p)", item->next);
    if (item->next != NULL)
    {
        Logger_cor("Free item->next.");
        item_dispose(item->next);
        Logger_cor("Set NULL to item->next.");
        item->next = NULL;
    }
    
    Logger_cor("Free item.");
    Memory_free(item);
    Logger_cor("Set NULL to item.");
    item = NULL;
    */
    
    
    
    Item next = NULL;
    
    Logger_cor("Check item. (%p)", item);
    if (item == NULL)
    {
        Logger_cor("item is NULL.");
        goto END;
    }
    
    
    Logger_cor("Get next item.");
    next = item->next;
    
    Logger_cor("Free item.");
    Memory_free(item);
    Logger_cor("Set NULL to item.");
    item = NULL;
    
    
    item_dispose(next);
    next = NULL;
    
END:
    Logger_cor("[  END  ]%s", __func__);
}



List
list_newFunction(
    char    *fileName,
    int     line
)
{
    List list = Memory_mallocAt(fileName, line, sizeof(struct ListTag));
    memset(list, 0x00, sizeof(struct ListTag));
    list->item = NULL;
    Logger_cor("&(list->item) = %p", &(list->item));
    list->last = NULL;
    return list;
}



void
list_dispose(
    List    list
)
{
    Logger_cor("[ START ]%s", __func__);
    Logger_cor("Check list. (%p)", list);
    if (list == NULL)
    {
        Logger_cor("list is NULL.");
        goto END;
    }
    
    Logger_cor("Check list->item. (%p)", list->item);
    if (list->item != NULL)
    {
        Logger_cor("Free list->item.");
        item_dispose(list->item);
        Logger_cor("Set NULL to list->item.");
        list->item = NULL;
    }
    
    Logger_cor("Free list.");
    Memory_free(list);
    Logger_cor("Set NULL to list.");
    list = NULL;
    
END:
    Logger_cor("[  END  ]%s", __func__);
}



void
list_addFunction(
    List    list,
    void    *object,
    char    *fileName,
    int     line
)
{
    Logger_cor("[ START ]%s", __func__);
    Logger_cor("list [%p]", list);
    if (list->item == NULL)
    {
        Logger_cor("list->item is NULL.");
        list->item = item_newFunction(object, fileName, line);
        Logger_cor("list->item is generated.");
        list->last = &(list->item);
        Logger_cor("Set list->last.");
        list->count++;
        Logger_cor("Increment list->count.");
        goto END;
    }
    
    
    Logger_cor("list->item is not NULL.");
    Item* lastItem = list->last;
    (*lastItem)->next = item_newFunction(object, fileName, line);
    list->last = &((*lastItem)->next);
    list->count++;
    
END:
    Logger_cor("[ START ]%s", __func__);
}



void*
list_get(
    List    list,
    int     index
)
{
    Item*   address = &(list->item);
    int     i = 0;
    
    for (i = 0; i < index; i++)
    {
        if (*address == NULL)
            return NULL;
        
        address = &((*address)->next);
    }
    
    if (&((*address)->object) == NULL)
        return NULL;
    
    return (*address)->object;
}



