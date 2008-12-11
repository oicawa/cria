#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_List.h"



Item
Item_newFunction(
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
Item_dispose(
    Item    item
)
{
    Logger_cor("[ START ]%s", __func__);
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
    
    
    Item_dispose(next);
    next = NULL;
    
END:
    Logger_cor("[  END  ]%s", __func__);
}



void*
Item_getObject(
	Item item
)
{
	return item->object;
}



void*
Item_getNext(
	Item item
)
{
	return item->next;
}



List
List_newFunction(
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
List_dispose(
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
        Item_dispose(list->item);
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
List_addFunction(
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
        list->item = Item_newFunction(object, fileName, line);
        Logger_cor("list->item is generated.");
        list->last = &(list->item);
        Logger_cor("Set list->last.");
        list->count++;
        Logger_cor("Increment list->count.");
        goto END;
    }
    
    
    Logger_cor("list->item is not NULL.");
    Item* lastItem = list->last;
    (*lastItem)->next = Item_newFunction(object, fileName, line);
    list->last = &((*lastItem)->next);
    list->count++;
    
END:
    Logger_cor("[ START ]%s", __func__);
}



void*
List_get(
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



int
List_count(
	List list
)
{
	return list->count;
}



Item
List_startItem(
    List    list
)
{
	return list->item;
}



