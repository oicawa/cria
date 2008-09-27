#ifndef PUBLIC_CORE_H_INCLUDED
#define PUBLIC_CORE_H_INCLUDED

#include <string.h>
#include "../Memory/Memory.h"
#include "../Logger/Logger.h"



//各インスタンスへのポインタ
typedef struct  IdentifierTag   Identifier; //全ての組み込みデータ型の先頭に確保される。ポインタにする必要はなし。
typedef struct  ObjectTag       *Object;
typedef struct  IntegerTag      *Integer;
typedef struct  BlockTag        *Block;
typedef struct  FileTag         *File;



//ファイルオープンモード
typedef enum {
    FILE_OPEN_MODE_READ,
    FILE_OPEN_MODE_WRITE,
    FILE_OPEN_MODE_READ_WRITE,
    FILE_OPEN_MODE_APPEND,
} FileOpenMode;



//==============================
//Object
//==============================
String
Object_toStringFunction(
    Object  object,
    char    *fileName,
    int     line
);
#define Object_toString(object)\
    (Object_toStringFunction(object, __FILE__, __LINE__))



//==============================
//IO
//==============================
void
IO_writeInteger(
    Integer integer
);



void
IO_writeString(
    String  string
);



void
IO_write(
    String format,
    ...
);



//==============================
//各種オブジェクトの生成
//==============================
Integer
Integer_newFunction(
    char    *input,
    char    *fileName,
    int     line
);
#define Integer_new(input)\
    (Integer_newFunction(input, __FILE__, __LINE__))



//Stringを生成
String
String_newFunction(
    char    *input,
    char    *fileName,
    int     line
);
#define String_new(string)\
    (String_newFunction(string, __FILE__, __LINE__))


//Listを生成



//==============================
//Integer
//==============================
//Decimalを破棄する
void
Integer_dispose(
    Integer integer
);



//文字列を取得
String
Integer_toStringFunction(
    Integer integer,
    char    *fileName,
    int     line
);
#define Integer_toString(integer)\
    (Integer_toStringFunction(integer, __FILE__, __LINE__))



//Integerを複製
Integer
Integer_cloneFunction(
    Integer source,
    char    *fileName,
    int     line
);
#define Integer_clone(source)\
    (Integer_cloneFunction(source, __FILE__, __LINE__))



//四則演算
//#define
Integer
Integer_calculateFunction(
    Integer ahead,
    Integer back,
    char    operation,
    char    *fileName,
    int     line
);
#define Integer_plus(ahead, back)\
    (Integer_calculateFunction(ahead, back, ('+'), __FILE__, __LINE__))
#define Integer_minus(ahead, back)\
    (Integer_calculateFunction(ahead, back, ('-'), __FILE__, __LINE__))
#define Integer_multiply(ahead, back)\
    (Integer_calculateFunction(ahead, back, ('*'), __FILE__, __LINE__))
#define Integer_divide(ahead, back)\
    (Integer_calculateFunction(ahead, back, ('/'), __FILE__, __LINE__))
#define Integer_modulo(ahead, back)\
    (Integer_calculateFunction(ahead, back, ('%'), __FILE__, __LINE__))



//==============================
//String
//==============================
//Stringを破棄する
void
String_dispose(
    String  string
);



//文字列長を取得
Integer
String_lengthFunction(
    String  string,
    char    *fileName,
    int     line
);
#define String_length(string)\
    (String_lengthFunction(string, __FILE__, __LINE__))



//連結した文字を返す
String
String_plusFunction(
    String  source,
    String  append,
    char    *fileName,
    int     line
);
#define String_plus(ahead, back)\
    (String_plusFunction(ahead, back, __FILE__, __LINE__))



//文字列を複製
String
String_cloneFunction(
    String  source,
    char    *fileName,
    int     line
);
#define String_clone(source)\
    (String_cloneFunction(source, __FILE__, __LINE__))



String
String_subStringFunction(
    String  string,
    Integer start,
    Integer size,
    char    *fileName,
    long    line
);
#define String_subString(string, start, size)\
    (String_subStringFunction(string, start, size, __FILE__, __LINE__))



//Stringの検索
Integer
String_findFunction(
    String  string,
    String  target,
    Integer offset,
    char    *fileName,
    int     line
);
#define String_find(string, target, offset)\
    (String_findFunction(string, target, offset, __FILE__, __LINE__))



List
String_splitFunction(
    String  string,
    String  separator,
    char    *fileName,
    long    line
);
#define String_split(string, separator)\
    (String_splitFunction(string, separator, __FILE__, __LINE__))



String
String_emptyFunction(
    char    *fileName,
    int     line
);
#define String_empty()\
    (String_emptyFunction(__FILE__, __LINE__))
    

//==============================
//File
//==============================
//Fileを生成
File
File_newFunction(
    char*           filePath,  //読み込むファイルパス
    FileOpenMode    mode,
    char*           fileName,
    int             line
);
#define File_new(filePath, mode)\
    (File_newFunction(filePath, mode, __FILE__, __LINE__))



//Fileをクローズする
void
File_dispose(
    File    file
);


//Fileの現在の文字を返す
String
File_readFunction(
    File    file,
    char*   fileName,
    int     line
);
#define File_read(file)\
    (File_readFunction(file, __FILE__, __LINE__))



//Fileの現在の行を返す
String
File_readLineFunction(
    File    file,
    char*   fileName,
    int     line
);
#define File_readLine(file)\
    (File_readLineFunction(file, __FILE__, __LINE__))



Boolean
File_isEndOfFile(
    File    file
);



/*
//Fileの現在の列位置を返す
int
File_getCurrentColumn(
    File  *file
);



//次の文字へ移動
void
File_move(
    File    *file,
    long    offset
);
*/



//==============================
//Native
//==============================
char*
Native_getCharPointer(
    String  string
);



long
Native_getLong(
    Integer integer
);



void
Native_print(
    char    *format,
    ...
);



#endif //PUBLIC_CORE_H_INCLUDED
