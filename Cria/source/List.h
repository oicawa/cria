#ifndef PUBLIC_LIST_H_INCLUDED
#define PUBLIC_LIST_H_INCLUDED


#include "Cria.h"



struct ItemTag {
    void *object;
    Item prev;
    Item next;
};



struct ListTag {
    Item item;
    Item last;
    int count;
};



Item
Item_new(
    void* object
);



void*
Item_getObject(
	Item item
);



void
Item_setObject(
	Item item,
    void* object
);



void*
Item_getNext(
	Item item
);



void
List_addFunction(
    List    list,
    void    *object,
    char    *fileName,
    int     line
);
#define List_add(list, object)\
    (List_addFunction(list, object, __FILE__, __LINE__))



Boolean
List_delete(
    List list,
    int index
);



Boolean
List_insert_item(
    List list,
    Item target,
    Item newItem
);



Boolean
List_insert(
    List list,
    int index,
    void* object
);



List
List_cat(
    List list1,
    List list2
);



void
List_append(
    List list1,
    List list2
);



void*
List_get_last(
    List    list
);



void
List_set(
    List    list,
    int     index,
    void* object
);



Item
List_startItem(
    List    list
);



Item
List_lastItem(
    List    list
);



char*
list_toStringFunction(
    List    list,
    char*   fileName,
    int     line
);
#define list_toString(list)\
    (list_toStringFunction(list, __FILE__, __LINE__))



Item
List_getItem(
    List list,
    int index
);



#endif
