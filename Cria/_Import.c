#include "Memory.h"
#include "Logger.h"

#include "String.h"
#include "StringBuffer.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "Parser.h"

#include "_Import.h"


Import
Import_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Import import = NULL;
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    StringBuffer package_name = NULL;
    StringBuffer library_name = NULL;
    
    
    Logger_dbg("Check TOKEN_TYPE_IMPORT.");
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IMPORT)
    {
        Logger_dbg("Not import token.");
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
            Logger_dbg("Not import token.");
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
    
    
    Logger_dbg("Create 'Import'");
    import = Memory_malloc(sizeof(struct ImportTag));
    import->package_name = StringBuffer_toString(package_name);
    import->library_name = StringBuffer_toString(library_name);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return import;
}



String
Import_get_package_name(
    Import import
)
{
    return import->package_name;
}
