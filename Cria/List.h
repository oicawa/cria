#ifndef PUBLIC_LIST_H_INCLUDED
#define PUBLIC_LIST_H_INCLUDED



typedef struct ItemTag *Item;
typedef struct ListTag *List;



void
Item_dispose(
    Item    item
);



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
List_dispose(
    List    list
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



char*
list_toStringFunction(
    List    list,
    char*   fileName,
    int     line
);
#define list_toString(list)\
    (list_toStringFunction(list, __FILE__, __LINE__))



#endif
