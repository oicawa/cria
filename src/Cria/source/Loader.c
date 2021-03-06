/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "String.h"
#include "StringBuffer.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Runtime.h"
#include "Definition.h"

#include "Loader.h"



String
Loader_create_path(
	List path,
	String separator
)
{
    Logger_trc("[ START ]%s", __func__);
    wchar_t buffer[MAX_PATH_LENGTH];
	int count = List_count(path);
	int index = 0;
	int end = 0;
	int length = 0;
	
	memset(buffer, 0x00, sizeof(buffer));
	
	count = List_count(path);
	index = 0;
	for (index = 0; index < count; index++)
	{
		wcscat(buffer, (String)(List_get(path, index)));
		wcscat(buffer, separator);
	}
	
	
	end = wcslen(buffer);
	length = wcslen(separator);
	memset(&buffer[end - length], 0x00, length);

	
    String full_path = String_new(buffer);
	
    Logger_trc("[  END  ]%s", __func__);
    return full_path;
}


Loader
Loader_new(
	List path
)
{
    Logger_trc("[ START ]%s", __func__);
    Loader loader = NULL;
	
	
    loader = Memory_malloc(sizeof(struct LoaderTag));
	loader->package_name = Loader_create_path(path, L"::");
    loader->library_name = Loader_create_path(path, L".");
	loader->library = NULL;
    

    Logger_trc("[  END  ]%s", __func__);
    return loader;
}



Loader
Loader_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Loader loader = NULL;
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    List path = NULL;
    

	Logger_dbg("Check TOKEN_TYPE_LOAD.");
    token = Parser_getCurrent(parser);
    Logger_dbg("Get current token. (token : %p / parser : %p)", token, parser);
    if (token->type != TOKEN_TYPE_LOAD)
    {
        Logger_dbg("Not load token.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
	Logger_dbg("Loop start.");
    path = List_new();
    while (TRUE)
    {
        Logger_dbg("Check package field");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (token->type != TOKEN_TYPE_CLASS_LITERAL)
        {
            Logger_dbg("Not load token.");
            Parser_setPosition(parser, position);
            Parser_error(parser, token);
            goto END;
        }
        
        
        List_add(path, token->value);
        
        
        Logger_dbg("Check colon or NEW_LINE");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_NEW_LINE)
        {
            break;
        }
        else if (token->type != TOKEN_TYPE_COLON)
        {
            Logger_dbg("Not colon token.");
            Parser_setPosition(parser, position);
            Parser_error(parser, token);
            goto END;
        }
        
        
        Logger_dbg("Check second colon");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (token->type != TOKEN_TYPE_COLON)
        {
            Logger_dbg("Not colon token.");
            Parser_setPosition(parser, position);
            Parser_error(parser, token);
            goto END;
        }
    }
    
    
    Parser_next(parser);
    
    
    Logger_dbg("Create 'Loader'");
    loader = Loader_new(path);
    
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
Loader_add_function(
    Interpreter interpreter,
    String functionName,
    CriaNativeFunction* functionPoint
)
{
    Logger_trc("[ START ]%s", __func__);
	Hash functions = Interpreter_functions(interpreter);
    DefinitionFunction definition = NULL;
    definition = DefinitionFunction_new(functionName, TRUE, TRUE, NULL, NULL, functionPoint);
    Hash_put(functions, functionName, definition);
    Logger_trc("[  END  ]%s", __func__);
}



void
Loader_add_class(
    Interpreter interpreter,
    String className,
    CriaNativeClassLoader* classLoader
)
{
    Logger_trc("[ START ]%s", __func__);
	Hash classes = Interpreter_classes(interpreter);
    DefinitionClass definition = NULL;
    definition = DefinitionClass_new(className, TRUE, NULL, NULL, NULL, NULL, classLoader);
    Hash_put(classes, className, definition);
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
Loader_load_native(
	Loader loader,
    Interpreter interpreter,
	String library_path
)
{
    Logger_trc("[ START ]%s", __func__);
	void *library = NULL;
	void (*Package_loader)(Interpreter);
	Boolean result = FALSE;
    
    
	library = dlopen(String_wcsrtombs(library_path), RTLD_LAZY);
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
	
	
	//Save library handler pointer
	loader->library = library;
	
	
	//Load package
	(Package_loader)(interpreter);
	
	if (loader == NULL)
	{
		Logger_dbg("Not native library.");
		goto END;
	}

	//!! "dlclose()" must be called in "Loader_unload_native()".
	
	
	result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



void
Loader_unload_native(
	Loader loader
)
{
    Logger_trc("[ START ]%s", __func__);
	if (loader == NULL)
	{
		Logger_dbg("Dose not exist loader object.");
		goto END;
	}
	
	if (loader->library == NULL)
	{
		Logger_dbg("Not native library.");
		goto END;
	}
	
	
	dlclose(loader->library);
	
END:
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
Loader_load_cria(
	Loader loader,
    Interpreter interpreter,
	String library_path
)
{
    Logger_trc("[ START ]%s", __func__);
	Boolean result = FALSE;
    List tokens = NULL;
    library_path = String_new(library_path);
    
	//Load target script file.
    tokens = Tokenizer_create_tokens(library_path, FALSE);
    if (tokens == NULL)
    {
        goto END;
    }
    
    if (Parser_create_syntax_tree(tokens, interpreter, library_path) == FALSE)
    {
        Logger_err("syntax parse error.");
        goto END;
    }
    
END:
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
    char current_path_buffer[MAX_PATH_LENGTH];
    char exefile_path_buffer[MAX_PATH_LENGTH];
    wchar_t wcbuffer[MAX_PATH_LENGTH];
	String cria_package_base = NULL;
	String current_package_base = NULL;
    
    
    //カレントディレクトリのパスを取得StringBuffer_toString(current_path)
    memset(current_path_buffer, 0x00, sizeof(current_path_buffer));
    getcwd(current_path_buffer, MAX_PATH_LENGTH);
	strcat(current_path_buffer, "/");
    
    
    //実行ファイルが存在するパスを取得
    //（※これで実行ファイルのフルパスは取得できるので、
    //　　あとはこのフルパスからディレクトリパスだけ取得すればいい。）
    memset(exefile_path_buffer, 0x00, sizeof(exefile_path_buffer));
    readlink("/proc/self/exe", exefile_path_buffer, MAX_PATH_LENGTH);
	size_t length = strlen(exefile_path_buffer);
    char* pointer = strrchr(exefile_path_buffer, '/');
    size_t index = pointer - exefile_path_buffer;
    memset(pointer + 1, 0x00, length - index);
    
    
    memset(wcbuffer, 0x00, sizeof(wcbuffer));
	wcscat(wcbuffer, (wchar_t*)String_mbsrtowcs(current_path_buffer));
	wcscat(wcbuffer, loader->library_name);
	current_package_base = String_new(wcbuffer);
    
    memset(wcbuffer, 0x00, sizeof(wcbuffer));
	wcscat(wcbuffer, (wchar_t*)String_mbsrtowcs(exefile_path_buffer));
    wcscat(wcbuffer, loader->library_name);
	cria_package_base = String_new(wcbuffer);
    
    
    memset(wcbuffer, 0x00, sizeof(wcbuffer));
    wcscat(wcbuffer, cria_package_base);
    wcscat(wcbuffer, L".so");
    if (Interpreter_has_loaded(interpreter, wcbuffer) == FALSE)
    {
        Interpreter_add_loaded_file(interpreter, wcbuffer);
	    Loader_load_native(loader, interpreter, wcbuffer);
    }

    memset(wcbuffer, 0x00, sizeof(wcbuffer));
    wcscat(wcbuffer, cria_package_base);
    wcscat(wcbuffer, L".ca");
    if (Interpreter_has_loaded(interpreter, wcbuffer) == FALSE)
    {
        Interpreter_add_loaded_file(interpreter, wcbuffer);
	    Loader_load_cria(loader, interpreter, wcbuffer);
    }

    memset(wcbuffer, 0x00, sizeof(wcbuffer));
    wcscat(wcbuffer, current_package_base);
    wcscat(wcbuffer, L".so");
    if (Interpreter_has_loaded(interpreter, wcbuffer) == FALSE)
    {
        Interpreter_add_loaded_file(interpreter, wcbuffer);
	    Loader_load_native(loader, interpreter, wcbuffer);
    }

    memset(wcbuffer, 0x00, sizeof(wcbuffer));
    wcscat(wcbuffer, current_package_base);
    wcscat(wcbuffer, L".ca");
    if (Interpreter_has_loaded(interpreter, wcbuffer) == FALSE)
    {
        Interpreter_add_loaded_file(interpreter, wcbuffer);
	    Loader_load_cria(loader, interpreter, wcbuffer);
    }	
	
	//Logger_err("Not found library named %s.", loader->library_name);
	//runtime_error(interpreter);

    
//END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}



void
Loader_unload(
	Loader loader
)
{
	Loader_unload_native(loader);
}

