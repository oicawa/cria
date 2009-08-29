/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
#include "Cria.h"
#include "Memory.h"
#include "Logger.h"

#include "List.h"



Item
Item_new(
    void    *object
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



void
Item_setObject(
	Item item,
    void* object
)
{
	item->object = object;
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
List_new(
    void
)
{
    List list = Memory_malloc(sizeof(struct ListTag));
    list->item = NULL;
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
        list->item = Item_new(object);
        list->last = list->item;
        list->count++;
        goto END;
    }
    
    
    Item last = list->last;
    Item newItem = Item_new(object);
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
List_insert_item(
    List list,
    Item target,
    Item newItem
)
{
    Logger_cor("[ START ]%s", __func__);
    Logger_cor("list [%p]", list);
    Item prev = NULL;
    Boolean result = FALSE;
    
    if (target == NULL)
    {
        result = FALSE;
        goto END;
    }
    
    prev = target->prev;
    
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
    Item newItem = NULL;
    Boolean result = FALSE;
    
    target = List_getItem(list, index);
    newItem = Item_new(object);
    
    result = List_insert_item(list, target, newItem);
    
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



void*
List_get_last(
    List    list
)
{
    return list->last->object;
}



List
List_cat(
    List list1,
    List list2
)
{
    int i = 0;
    List list = List_new();
    
    if (list1 != NULL)
    {
        for (i = 0; i < List_count(list1); i++)
        {
            List_add(list, List_get(list1, i));
        }
    }
    
    if (list2 != NULL)
    {
        for (i = 0; i < List_count(list2); i++)
        {
            List_add(list, List_get(list2, i));
        }
    }
    
    return list;
}



void
List_append(
    List list1,
    List list2
)
{
    int i = 0;
    
    if (list1 == NULL)
        return;
    
    if (list2 == NULL)
        return;
    
    for (i = 0; i < List_count(list2); i++)
    {
        List_add(list1, List_get(list2, i));
    }
}



void
List_set(
    List list,
    int index,
    void* value
)
{
    Item target = List_getItem(list, index);
    
    if (target == NULL)
        goto END;
    
    target->object = value;
    
END:
    return;
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



