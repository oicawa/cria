#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "DefinitionFunction.h"
#include "DefinitionClass.h"
#include "IO.h"
#include "CriaFile.h"
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
    list_add(interpreter->functions, definition);
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
    definition = DefinitionClass_new(interpreter, className, TRUE, NULL, NULL, classLoader);
    list_add(interpreter->classes, definition);
    Logger_trc("[  END  ]%s", __func__);
}



void
interpreter_loadCore(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    Interpreter_addFunction(interpreter, "write", io_write);
    Interpreter_addFunction(interpreter, "read", io_read);
    Interpreter_addClass(interpreter, "File", CriaFile_loadClass);
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
    Logger_dbg("interpreter->statementList is [%p]", interpreter->statements);
    interpreter->variables = List_new();
    interpreter->functions = List_new();
    interpreter->classes = List_new();
    interpreter->row = 0;
    interpreter->column = 0;
    interpreter->indentLevel = 0;
    
    interpreter_loadCore(interpreter);
    
    return interpreter;
}



void
Interpreter_dispose(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    if (interpreter == NULL)
    {
        goto END;
    }
    
    Logger_dbg("interpreter->statements is [%p]", interpreter->statements);
    if (interpreter->statements != NULL)
    {
        list_dispose(interpreter->statements);
        interpreter->statements = NULL;
    }
    
    interpreter = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
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
    
    if (parser_create_syntax_tree(tokens, interpreter) == FALSE)
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



List
Interpreter_classes(
	Interpreter interpreter
)
{
	return interpreter->classes;
}



List
Interpreter_functions(
	Interpreter interpreter
)
{
	return interpreter->functions;
}



List
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
