#include <stdio.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"



#define READ_LINE_BUFFER_SIZE   256



static char DATA_TYPE_FILE[] = "File";




//Fileを生成
File
file_newFunction(
    char*           filePath,  //読み込むファイルパス
    FileOpenMode    mode,
    char*           fileName,
    int             line
)
{
    File    file = NULL;
    FILE    *pointer = NULL;
    
    pointer = fopen(filePath, "r");
    if (pointer == NULL)
    {
        Logger_err("fopen error.");
        return NULL;
    }
    file = Memory_mallocAt(fileName, line, sizeof(struct FileTag));
    memset(file, 0x00, sizeof(struct FileTag));
    file->pointer = pointer;
    file->path = Memory_mallocAt(fileName, line, strlen(filePath) + 1);
    memset(file->path, 0x00, strlen(filePath) + 1);
    strcpy(file->path, filePath);
    file->charactor = '\0';
    file->line = 1;
    file->column = 0;
    return file;
}



//Fileをクローズして破棄
void
File_dispose(
    File    file
)
{
    Logger_cor("[ START ]%s", __func__);
    if (file == NULL)
    {
        Logger_dbg("file is NULL.");
        goto END;
    }
    
    Logger_dbg("Check file->pointer.");
    if (file->pointer != NULL)
    {
        Logger_dbg("fclose file->pointer.");
        fclose(file->pointer);
        file->pointer = NULL;
    }
    
    Logger_dbg("Check file->path.");
    if (file->path != NULL)
    {
        Logger_dbg("Free file->path.");
        Memory_free(file->path);
        file->path = NULL;
    }
    
    Logger_dbg("Free file.");
    Memory_free(file);
    Logger_dbg("Set NULL to file.");
    file = NULL;
    
END:
    Logger_cor("[ START ]%s", __func__);
}



//ファイルが終端に達したかどうか。
Boolean
file_isEndOfFile(
    File    file
)
{
    if (feof(file->pointer) == 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



//Fileの現在の列位置を返す
long
file_getColumnNumber(
    File    file
)
{
    return file->column;
}



//Fileの現在の行番号を返す
long
file_getLineNumber(
    File    file
)
{
    return file->line;
}



//ファイル名取得
char*
file_getPath(
    File    file
)
{
    return file->path;
}



char*
file_getDataType(
    void
)
{
    return DATA_TYPE_FILE;
}



