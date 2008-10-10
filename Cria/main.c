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
    //Logger_set(LOG_LEVEL_DEBUG, "addStatement, Interpreter_compile");
    //Logger_set(LOG_LEVEL_DEBUG, "token_log");
    //Logger_set(LOG_LEVEL_DEBUG, "parseIndentDident");
    //Logger_set(LOG_LEVEL_DEBUG, "parseClassOrConstant");
    //Logger_set(LOG_LEVEL_DEBUG, "parseSpace,skipSpaceAndNewLine");
    //Logger_set(LOG_LEVEL_DEBUG, "parseOther");
    //Logger_set(LOG_LEVEL_DEBUG, "parser_parse, statement_parse, functionCallStatement_parse, functionCallExpression_parse, token_log");
    //Logger_set(LOG_LEVEL_CORE, "Interpreter_dispose, Interpreter_compile, tokenizer_dispose, parser_dispose, List_dispose, addStatement, List_addFunction");
    //Logger_set(LOG_LEVEL_DEBUG, "parseSubstituteStatement, parseParametersExpression, io_print, evaluator_functionCall, evaluator_parameters, evaluator_stringLiteral, ");
    //Logger_set(LOG_LEVEL_DEBUG, "parser_parse, parser_next, logAllTokens, statement_parse, parseFunctionCallExpression, parseParametersExpression, parseFunctionCallStatement, parseFactor, expression_parseReferenceExpression");
    //Logger_set(LOG_LEVEL_DEBUG, "parseAndOr, parseNotEqual, parseCompare, parsePlusMinus, parseMultiplyDivide, parseFactor, expression_parseReferenceExpression, parseGenerateExpression, parseFunctionCallExpression, parseParametersExpression, parseClassExpression, parseVariableExpression, ");
    //Logger_set(LOG_LEVEL_DEBUG, "parseFunctionCallExpression, parseParametersExpression, expression_parseReferenceExpression");
    Logger_set(LOG_LEVEL_DEBUG, "evaluator_expression, evaluator_operation, evaluator_parameters, evaluator_functionCall, evaluator_reference, evaluator_stringLiteral, evaluator_integerLiteral");
    //Logger_set(LOG_LEVEL_DEBUG, "evaluator_operation, parseNumber, evaluator_parameters, parseFactor, io_write");
    //Logger_set(LOG_LEVEL_TRACE, NULL);
    
    
    
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
