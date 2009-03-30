#include <unistd.h>
#include <memory.h>
#include <dlfcn.h>

#include "Memory.h"
#include "Logger.h"
#include "String.h"
#include "StringBuffer.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Runtime.h"
#include "Boolean.h"

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
	StringBuffer function_name = NULL;
    
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
    function_name = StringBuffer_new();
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
        StringBuffer_append(function_name, Token_buffer(token));
        
        
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
        StringBuffer_append(function_name, "_");
    }
    
    
    Parser_next(parser);
    
    
    Logger_dbg("Create 'Loader'");
    loader = Memory_malloc(sizeof(struct LoaderTag));
    loader->package_name = StringBuffer_toString(package_name);
    loader->library_name = StringBuffer_toString(library_name);
    loader->function_name = StringBuffer_toString(function_name);
    
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


Boolean
Loader_load_native(
    Interpreter interpreter,
	String library_path
)
{
    Logger_trc("[ START ]%s", __func__);
	void *library = NULL;
	void (*Package_loader)(Interpreter);
	Boolean result = FALSE;
    
    
	library = dlopen(library_path, RTLD_LAZY);
	if (library == NULL)
	{
		Logger_err("%s", dlerror());
		goto END;
	}


	Package_loader = dlsym (library, "__Package_load__");
	if (Package_loader == NULL)
	{
		Logger_err("%s", dlerror());
		goto END;
	}
	
	
	(Package_loader)(interpreter);
	
	
	dlclose(library);
	result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
Loader_load_cria(
    Interpreter interpreter,
	String library_path
)
{
    Logger_trc("[ START ]%s", __func__);
	Boolean result = FALSE;
    
//END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}

void
Loader_load(
    Loader loader,
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    char buffer[MAX_PATH_LENGTH];
    StringBuffer cria_home_path = StringBuffer_new();
    StringBuffer current_path = StringBuffer_new();
	String cria_package_base = NULL;
	String current_package_base = NULL;
	Boolean result = FALSE;
    
    
    //カレントディレクトリのパスを取得StringBuffer_toString(current_path)
    memset(buffer, 0x00, MAX_PATH_LENGTH);
    getcwd(buffer, MAX_PATH_LENGTH);
    StringBuffer_append(current_path, buffer);
    StringBuffer_append(current_path, "/");
    StringBuffer_append(current_path, loader->library_name);
	current_package_base = StringBuffer_toString(current_path);
    printf("*** [%s] ***\n", current_package_base);
    
    
    //実行ファイルが存在するパスを取得
    //（※これで実行ファイルのフルパスは取得できるので、
    //　　あとはこのフルパスからディレクトリパスだけ取得すればいい。）
    memset(buffer, 0x00, MAX_PATH_LENGTH);
    readlink("/proc/self/exe", buffer, MAX_PATH_LENGTH);
    

    //ここにディレクトリパスだけ取得する処理を。。
    size_t length = strlen(buffer);
    char* pointer = strrchr(buffer, '/');
    size_t index = pointer - buffer;
    memset(pointer + 1, 0x00, length - index);
    
    
    StringBuffer_append(cria_home_path, buffer);
    StringBuffer_append(cria_home_path, loader->library_name);
	cria_package_base = StringBuffer_toString(cria_home_path);
    printf("*** [%s] ***\n", cria_package_base);
    
    
    memset(buffer, 0x00, MAX_PATH_LENGTH);
    strcat(buffer, cria_package_base);
    strcat(buffer, ".so");
	result = Loader_load_native(interpreter, buffer);
	if (result == TRUE)
	{
		Logger_dbg("Loaded native package. [%s]", buffer);
		goto END;
	}


    memset(buffer, 0x00, MAX_PATH_LENGTH);
    strcat(buffer, cria_package_base);
    strcat(buffer, ".ca");
	result = Loader_load_cria(interpreter, buffer);
	if (result == TRUE)
	{
		Logger_dbg("Loaded cria package. [%s]", buffer);
		goto END;
	}


    memset(buffer, 0x00, MAX_PATH_LENGTH);
    strcat(buffer, current_package_base);
    strcat(buffer, ".so");
	result = Loader_load_native(interpreter, buffer);
	if (result == TRUE)
	{
		Logger_dbg("Loaded native package. [%s]", buffer);
		goto END;
	}


    memset(buffer, 0x00, MAX_PATH_LENGTH);
    strcat(buffer, current_package_base);
    strcat(buffer, ".ca");
	result = Loader_load_cria(interpreter, buffer);
	if (result == TRUE)
	{
		Logger_dbg("Loaded cria package. [%s]", buffer);
		goto END;
	}
	
	
	Logger_err("Not found library named %s.", loader->library_name);
	runtime_error(interpreter);

    
END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}
