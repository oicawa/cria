#include <unistd.h>
#include <memory.h>

#include "Memory.h"
#include "Logger.h"

#include "String.h"
#include "StringBuffer.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "Parser.h"

#include "_Loader.h"


Loader
Loader_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Loader loader = NULL;
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    StringBuffer package_name = NULL;
    StringBuffer library_name = NULL;
    
    Logger_dbg("Check TOKEN_TYPE_LOAD.");
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_LOAD)
    {
        Logger_dbg("Not load token.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
    package_name = StringBuffer_new();
    library_name = StringBuffer_new();
    while (TRUE)
    {
        Logger_dbg("Check package field");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (Token_type(token) != TOKEN_TYPE_CLASS_LITERAL)
        {
            Logger_dbg("Not load token.");
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
        
        
        StringBuffer_append(package_name, Token_buffer(token));
        StringBuffer_append(library_name, Token_buffer(token));
        
        
        Logger_dbg("Check colon or NEW_LINE");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (Token_type(token) == TOKEN_TYPE_NEW_LINE)
        {
            break;
        }
        else if (Token_type(token) != TOKEN_TYPE_COLON)
        {
            Logger_dbg("Not colon token.");
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
        
        
        Logger_dbg("Check second colon");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (Token_type(token) != TOKEN_TYPE_COLON)
        {
            Logger_dbg("Not colon token.");
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
        
        
        StringBuffer_append(package_name, "::");
        StringBuffer_append(library_name, ".");
    }
    
    
    Parser_next(parser);
    
    
    Logger_dbg("Create 'Loader'");
    loader = Memory_malloc(sizeof(struct LoaderTag));
    loader->package_name = StringBuffer_toString(package_name);
    loader->library_name = StringBuffer_toString(library_name);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return loader;
}



String
Loader_get_package_name(
    Loader loader
)
{
    return loader->package_name;
}


void
Loader_load(
    Loader loader,
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    char buffer[MAX_PATH_LENGTH];
    
    memset(buffer, 0x00, MAX_PATH_LENGTH);
    
    //カレントディレクトリのパスを取得
    //getcwd関数を使用
    
    //実行ファイルが存在するパスを取得
    //#include <unistd.h>
    readlink("/proc/self/exe", buffer, MAX_PATH_LENGTH);
    printf("***** [%s] *****\n\n\n", buffer);
    
    
    
//END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}

