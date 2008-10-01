#include <stdio.h>

#include "_Cria.h"



//========================================
//Main
//========================================
int
main(
    int     argc,
    char**  argv
)
{
    //Logger_set(LOG_LEVEL_CORE, "stringBuffer_appendChar,stringBuffer_append");
    //Logger_set(LOG_LEVEL_CORE, "StringBuffer_dispose");
    //Logger_set(LOG_LEVEL_CORE, "addStatement, List_addFunction");
    //Logger_set(LOG_LEVEL_CORE, "StringBuffer_dispose, List_dispose, String_dispose, item_dispose");
    //Logger_set(LOG_LEVEL_DEBUG, NULL);
    //Logger_set(LOG_LEVEL_TRACE, NULL);
    //Logger_set(LOG_LEVEL_DEBUG, "addStatement, Interpreter_compile");
    //Logger_set(LOG_LEVEL_DEBUG, "token_log");
    //Logger_set(LOG_LEVEL_DEBUG, "parseIndentDident");
    //Logger_set(LOG_LEVEL_DEBUG, "parseClassOrConstant");
    //Logger_set(LOG_LEVEL_DEBUG, "parseSpace,skipSpaceAndNewLine");
    //Logger_set(LOG_LEVEL_DEBUG, "parseOther");
    //Logger_set(LOG_LEVEL_DEBUG, "parser_parse, statement_parse, functionCallStatement_parse, functionCallExpression_parse, token_log");
    //Logger_set(LOG_LEVEL_CORE, "Interpreter_dispose, Interpreter_compile, tokenizer_dispose, parser_dispose, List_dispose, addStatement, List_addFunction");
    //Logger_set(LOG_LEVEL_DEBUG, "io_print, expression_new, expression_parse, evaluator_evaluateFunctionCallExpression, evaluator_evaluateParametersExpression, evaluator_evaluateStringLiteralExpression, ");
    Logger_set(LOG_LEVEL_DEBUG, "executor_executeStatement, executor_executeFunctionCallStatement, evaluator_evaluateStringLiteralExpression, evaluator_evaluateParametersExpression, evaluator_evaluateFunctionCallExpression, io_write, io_read");
    
    
    
    //*
    int endCode = 0;
    
    Interpreter interpreter = NULL;
    int result = 0;
    
    if (argv[1] == NULL)
    {
        printf("Input cria script file path.\n");
        goto END;
    }
    
    
    interpreter = Interpreter_new();
    if (interpreter == NULL)
    {
        printf("Cria interpreter create error.\n");
        goto END;
    }
    
    
    result = Interpreter_compile(interpreter, argv[1]);
    if (result == FALSE)
    {
        printf("Cria interpreter compile error.\n");
        goto END;
    }
    
    
    Interpreter_run(interpreter);
    
    
    endCode = 1;
    
END:
    //Memory_dumpBlocks(stdout);
    Interpreter_dispose(interpreter);
    interpreter = NULL;
    
    
    //printf("!!! Memory_dispose() !!!\n");
    Memory_dispose();
    
    return endCode;
    //*/
}
