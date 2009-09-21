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
#include "Definition.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Statement.h"
#include "Loader.h"
#include "CriaIO.h"
#include "CriaString.h"
#include "CriaFile.h"
#include "CriaList.h"
#include "CriaInteger.h"
#include "Runtime.h"
#include "String.h"

#include "Interpreter.h"



List
Interpreter_stack(
    Interpreter interpreter
)
{
    return interpreter->stack;
}



void
Interpreter_stack_add(
    Interpreter interpreter,
    void* expression
)
{
    List_add(interpreter->stack, expression);
}



void
Interpreter_stack_pop(
    Interpreter interpreter
)
{
    int count = List_count(interpreter->stack);
    List_delete(interpreter->stack, count - 1);
}


Interpreter
Interpreter_new(
    void
)
{
    Interpreter interpreter = NULL;
    
    interpreter = Memory_malloc(sizeof(struct InterpreterTag));
    interpreter->statements = List_new();
    interpreter->libraries = Hash_new(16);
    interpreter->variables = Hash_new(32);
    interpreter->functions = Hash_new(64);
    interpreter->classes = Hash_new(64);
    interpreter->row = 0;
    interpreter->column = 0;
    interpreter->indentLevel = 0;
    interpreter->loaded_files = Hash_new(16);
    interpreter->stack = List_new();
    
    return interpreter;
}



Boolean
Interpreter_compile(
    Interpreter interpreter,
    String filePath
)
{
    Logger_trc("[ START ]%s", __func__);
    Boolean result = FALSE;
    List tokens = NULL;
	
	Loader_add_function(interpreter, L"write", CriaIO_write);
	Loader_add_function(interpreter, L"read", CriaIO_read);
	Loader_add_class(interpreter, L"String", CriaString_loadClass);
	Loader_add_class(interpreter, L"File", CriaFile_loadClass);
	Loader_add_class(interpreter, L"List", CriaList_loadClass);
	Loader_add_class(interpreter, L"Integer", CriaInteger_loadClass);
    
	
	//Load target script file.
    tokens = Tokenizer_create_tokens(filePath, TRUE);
    if (tokens == NULL)
    {
        Runtime_error(interpreter, "Creating tokens failured. [%s]", filePath);
        goto END;
    }
    
    
    if (Parser_create_syntax_tree(tokens, interpreter, filePath) == FALSE)
    {
        Logger_err("syntax parse error.");
        goto END;
    }
    
    result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



void
Interpreter_run(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    
    Statement_executeList(interpreter, NULL, NULL, NULL, interpreter->statements);
    
    Logger_trc("[  END  ]%s", __func__);
}



int
Interpreter_row(
	Interpreter interpreter
)
{
	return interpreter->row;
}



Hash
Interpreter_classes(
	Interpreter interpreter
)
{
	return interpreter->classes;
}



Hash
Interpreter_functions(
	Interpreter interpreter
)
{
	return interpreter->functions;
}



Hash
Interpreter_variables(
	Interpreter interpreter
)
{
	return interpreter->variables;
}



List
Interpreter_statements(
	Interpreter interpreter
)
{
	return interpreter->statements;
}



void
Interpreter_setRow(
	Interpreter interpreter,
	int row
)
{
	interpreter->row = row;
}



Hash
Interpreter_get_libraries(
    Interpreter interpreter
)
{
    return interpreter->libraries;
}


void
Interpreter_dispose(
	Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    
	Hash hash = interpreter->libraries;
    List list = Hash_get_keys(hash);
	int count = List_count(list);
	int index = 0;
	
	for (index = 0; index < count; index++)
	{
		String key = (String)List_get(list, index);
		Loader loader = (Loader)Hash_get(hash, key);
		Loader_unload(loader);
	}
    
    Logger_trc("[  END  ]%s", __func__);
}


Boolean
Interpreter_has_loaded(
    Interpreter interpreter,
    String file_path
)
{
    Logger_trc("[ START ]%s", __func__);
    
	Hash hash = interpreter->loaded_files;
    void* value = Hash_get(hash, file_path);
    Boolean result = FALSE;
    
    if (value != NULL)
        result = TRUE;
    
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



void
Interpreter_add_loaded_file(
    Interpreter interpreter,
    String file_path
)
{
    Logger_trc("[ START ]%s", __func__);
    
	Hash hash = interpreter->loaded_files;
    Hash_put(hash, String_new(file_path), String_new(file_path));
    
    Logger_trc("[  END  ]%s", __func__);
    return;
}


