#include "_Cria.h"

void
Interpreter_addFunction(
    Interpreter interpreter,
    char* functionName,
    CriaNativeFunction* functionPoint
)
{
    Logger_trc("[ START ]%s", __func__);
    FunctionDefinition definition = NULL;
    definition = functionDefinition_new(functionName, TRUE, NULL, NULL, functionPoint);
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
    ClassDefinition definition = NULL;
    definition = classDefinition_new(interpreter, className, TRUE, NULL, NULL, classLoader);
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
    interpreter->statements = list_new();
    Logger_dbg("interpreter->statementList is [%p]", interpreter->statements);
    interpreter->variables = list_new();
    interpreter->functions = list_new();
    interpreter->classes = list_new();
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
    
    tokens = tokenizer_create_tokens(filePath);
    
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



//実行
void
Interpreter_run(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    
    executor_executeStatementList(interpreter, NULL, NULL, interpreter->statements);
    
    Logger_trc("[  END  ]%s", __func__);
}



