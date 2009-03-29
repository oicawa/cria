#include "Memory.h"
#include "Logger.h"

#include "Boolean.h"

#include "_List.h"



Item
Item_newFunction(
    void    *object,
    char    *fileName,
    int     line
)
{
    Item item = Memory_malloc(sizeof(struct ItemTag));
    item->object = object;
    item->prev = NULL;
    item->next = NULL;
    return item;
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



void*
Item_getPrev(
	Item item
)
{
	return item->prev;
}



List
List_newFunction(
    char    *fileName,
    int     line
)
{
    List list = Memory_malloc(sizeof(struct ListTag));
    list->item = NULL;
    Logger_cor("&(list->item) = %p", &(list->item));
    list->last = NULL;
    return list;
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
        list->item = Item_newFunction(object, fileName, line);
        list->last = list->item;
        list->count++;
        goto END;
    }
    
    
    Item last = list->last;
    Item newItem = Item_newFunction(object, fileName, line);
    newItem->prev = last;
    last->next = newItem;
    list->last = newItem;
    list->count++;
    
END:
    Logger_cor("[ START ]%s", __func__);
}



Item
List_getItem(
    List list,
    int index
)
{
    Logger_cor("[ START ]%s", __func__);
    Item current = NULL;
    int i = 0;
    
    if (list->count < index)
    {
        Logger_err("Specified index('%d') does not exist in target list. (last index is '%d')", index, list->count);
        goto END;
    }
    
    current = list->item;
    for (i = 0; i < index; i++)
    {
        current = current->next;
    }
    
END:
    Logger_cor("[ START ]%s", __func__);
    return current;
}



Boolean
List_delete(
    List list,
    int index
)
{
    Logger_cor("[ START ]%s", __func__);
    Logger_cor("list [%p]", list);
    Item target = NULL;
    Item prev = NULL;
    Item next = NULL;
    Boolean result = FALSE;
    
    target = List_getItem(list, index);
    if (target == NULL)
    {
        result = FALSE;
        goto END;
    }
    
    prev = target->prev;
    next = target->next;
    
    if (prev == NULL)
        list->item = next;
    else
        prev->next = next;
    
    if (next == NULL)
        list->last = prev;
    else
        next->prev = prev;
    
    list->count--;
    
    result = TRUE;
    
END:
    Logger_cor("[ START ]%s", __func__);
    return result;
}



Boolean
List_insert(
    List list,
    int index,
    void* object
)
{
    Logger_cor("[ START ]%s", __func__);
    Logger_cor("list [%p]", list);
    Item target = NULL;
    Item prev = NULL;
    Item newItem = NULL;
    Boolean result = FALSE;
    
    target = List_getItem(list, index);
    if (target == NULL)
    {
        result = FALSE;
        goto END;
    }
    
    prev = target->prev;
    newItem = Item_newFunction(object, __FILE__, __LINE__);
    
    target->prev = newItem;
    newItem->next = target;
    
    if (prev == NULL)
    {
        list->item = newItem;
    }
    else
    {
        prev->next = newItem;
        newItem->prev = prev;
    }
    
    
    list->count++;
    
    result = TRUE;
    
END:
    Logger_cor("[ START ]%s", __func__);
    return result;
}



void*
List_get(
    List    list,
    int     index
)
{
    Item target = List_getItem(list, index);
    void* object = NULL;
    
    if (target == NULL)
        goto END;
    
    object = target->object;
    
END:
    return object;
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



Item
List_lastItem(
    List    list
)
{
	return list->last;
}



