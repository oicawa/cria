#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "Definition.h"
#include "CriaIO.h"
#include "CriaFile.h"
#include "CriaList.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Statement.h"

#include "_Interpreter.h"

void
Interpreter_addFunction(
    Interpreter interpreter,
    char* functionName,
    CriaNativeFunction* functionPoint
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction definition = NULL;
    definition = DefinitionFunction_new(functionName, TRUE, TRUE, NULL, NULL, functionPoint);
    Hash_put(interpreter->functions, functionName, definition);
    Logger_trc("[  END  ]%s", __func__);
}



void
Interpreter_addClass(
    Interpreter interpreter,
    char* className,
    CriaNativeClassLoader* classLoader
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionClass definition = NULL;
    definition = DefinitionClass_new(className, TRUE, NULL, NULL, NULL, NULL, classLoader);
    Hash_put(interpreter->classes, className, definition);
    Logger_trc("[  END  ]%s", __func__);
}



void
Interpreter_loadCore(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    Interpreter_addFunction(interpreter, "write", CriaIO_write);
    Interpreter_addFunction(interpreter, "read", CriaIO_read);
    Interpreter_addClass(interpreter, "File", CriaFile_loadClass);
    Interpreter_addClass(interpreter, "List", CriaList_loadClass);
    Logger_trc("[  END  ]%s", __func__);
}



Interpreter
Interpreter_new(
    void
)
{
    Interpreter interpreter = NULL;
    
    interpreter = Memory_malloc(sizeof(struct InterpreterTag));
    interpreter->statements = List_new();
    Logger_dbg("interpreter->statements is [%p]", interpreter->statements);
    interpreter->variables = Hash_new(32);
    interpreter->functions = Hash_new(64);
    interpreter->classes = Hash_new(64);
    interpreter->row = 0;
    interpreter->column = 0;
    interpreter->indentLevel = 0;
    
    Interpreter_loadCore(interpreter);
    
    return interpreter;
}



Boolean
Interpreter_compile(
    Interpreter interpreter,
    char* filePath
)
{
    Logger_trc("[ START ]%s", __func__);
    Boolean result = FALSE;
    List tokens = NULL;
    
    tokens = Tokenizer_create_tokens(filePath);
    
    if (Parser_create_syntax_tree(tokens, interpreter) == FALSE)
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
    
    Statement_executeList(interpreter, NULL, NULL, interpreter->statements);
    
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
