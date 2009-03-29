#ifndef PUBLIC_LIST_H_INCLUDED
#define PUBLIC_LIST_H_INCLUDED


#include "Boolean.h"

typedef struct ItemTag *Item;
typedef struct ListTag *List;



void*
Item_getObject(
	Item item
);



void*
Item_getNext(
	Item item
);



List
List_newFunction(
    char    *fileName,
    int     line
);
#define List_new()\
    (List_newFunction(__FILE__, __LINE__))



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
List_insert(
    List list,
    int index,
    void* object
);



void*
List_get(
    List    list,
    int     index
);



int
List_count(
    List    list
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



#endif
