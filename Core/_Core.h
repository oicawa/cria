#ifndef PRIVATE_CORE_H_INCLUDED
#define PRIVATE_CORE_H_INCLUDED



#include "Core.h"



//Identifier構造体
struct IdentifierTag {
    char    *type;
    char    *name;
};



//Object構造体
struct ObjectTag {
    Identifier  id;
    void        *object;
};



//Integer構造体
struct IntegerTag {
    Identifier  id;
    long        value;
};



//String構造体
struct StringTag {
    Identifier  id;
    char*       pointer;
};



//File構造体
struct FileTag {
    Identifier  id;
    FILE*       pointer;
    char*       path;
    int         charactor;
    long        line;
    long        column;
};



//==============================
//Coreパッケージ内で共有される
//関数のプロトタイプ宣言
//==============================
char*
integer_getDataType(
    void
);



long
string_length(
    String  string
);



char*
string_getDataType(
    void
);



char*
string_getEmpty(
    void
);



char*
list_getDataType(
    void
);



int
file_read(
    File    file
);



long
file_getColumnNumber(
    File    file
);



long
file_getLineNumber(
    File    file
);



char*
file_getDataType(
    void
);



char*
stringBuffer_getDataType(
    void
);



void
stringBuffer_append(
    StringBuffer    stringBuffer,
    char*           string,
    char*           fileName,
    int             line
);



void
stringBuffer_appendChar(
    StringBuffer    stringBuffer,
    char            charactor,
    char*           fileName,
    int             line
);



#endif /* PRIVATE_CORE_H_INCLUDED */

