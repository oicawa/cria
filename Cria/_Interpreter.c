#include "_Cria.h"

//==================================================
//Interpreter
//==================================================
/*
void
Interpreter_loadFunction(
    Interpreter             interpreter,
    char*                   moduleName,
    char*                   functionName
)
{
    FunctionDefinition  funcDef = NULL;
    funcDef = functionDefinition_new(moduleName, functionName, TRUE);
    list_add(interpreter->functionList, funcDef);
}
*/


void
Interpreter_addFunction(
    Interpreter         interpreter,
    char*               functionName,
    CriaNativeFunction* functionPoint
)
{
    Logger_trc("[ START ]%s", __func__);
    FunctionDefinition definition = NULL;
    definition = functionDefinition_new(functionName, TRUE, ACCESS_LEVEL_PUBLIC, NULL, NULL, functionPoint);
    list_add(interpreter->functionList, definition);
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
    Logger_trc("[  END  ]%s", __func__);
}



Interpreter
Interpreter_new(
    void
)
{
    Interpreter interpreter = NULL;
    
    interpreter = Memory_malloc(sizeof(struct InterpreterTag));
    interpreter->statementList = list_new();
    Logger_dbg("interpreter->statementList is [%p]", interpreter->statementList);
    interpreter->variableList = list_new();
    interpreter->functionList = list_new();
    //interpreter->classList = List_new();
    interpreter->buffer = NULL;
    interpreter->lineNumber = 0;
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
    
    Logger_dbg("interpreter->statementList is [%p]", interpreter->statementList);
    if (interpreter->statementList != NULL)
    {
        list_dispose(interpreter->statementList);
        interpreter->statementList = NULL;
    }
    
    interpreter = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
Interpreter_compile(
    Interpreter interpreter,
    char*       filePath
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean     result = FALSE;
    List    tokens = NULL;
    Parser      parser = NULL;
    
//    tokenizer = tokenizer_new(filePath);
//    if (tokenizer == NULL)
//    {
//        Logger_err("Create Tokenizer error.");
//        goto END;
//    }
//    
//    //字句解析
//    if (tokenizer_do(tokenizer) == FALSE)
//    {
//        Logger_err("token parse error.");
//        goto END;
//    }
    
    tokens = tokenizer_createTokens(filePath);
    
    //構文解析
    parser = parser_new(tokens);
    if (parser_parse(parser, interpreter) == FALSE)
    {
        Logger_err("syntax parse error.");
        goto END;
    }
    
    //処理成功
    result = TRUE;
    
END:
    //開放
    parser_dispose(parser);
    parser = NULL;
    
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
    
    executor_executeStatementList(interpreter, interpreter->statementList);
    
    Logger_trc("[  END  ]%s", __func__);
}



