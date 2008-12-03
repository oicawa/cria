#ifndef PRIVATE_LIST_H_INCLUDED
#define PRIVATE_LIST_H_INCLUDED



typedef struct ItemTag *Item;
struct ItemTag {
    void    *object;
    Item    next;
};



typedef struct ListTag {
    Item        item;
    Item*       last;
    int         count;
} *List;



Item
item_newFunction(
    void    *object,
    char    *fileName,
    int     line
);



void
item_dispose(
    Item    item
);



List
list_newFunction(
    char    *fileName,
    int     line
);
#define list_new()\
    (list_newFunction(__FILE__, __LINE__))



void
list_dispose(
    List    list
);



void
list_addFunction(
    List    list,
    void    *object,
    char    *fileName,
    int     line
);
#define list_add(list, object)\
    (list_addFunction(list, object, __FILE__, __LINE__))



void*
list_get(
    List    list,
    int     index
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
