#include <stdio.h>

#include "_Cria.h"



CriaId
evaluator_evaluateStringLiteralExpression(
    Interpreter             interpreter,
    StringLiteralExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString string = NULL;
    //long length = string_length(expression->value);
    char* buffer = expression->value->pointer;
    char* start = &buffer[1];   //先頭の'"'は読み飛ばす
    char* next = NULL;
    StringBuffer stringBuffer = stringBuffer_new();
    
    
    Logger_dbg("Literal is '%s'", buffer);
    
    while (*start != '\0')
    {
        Logger_dbg("literal check");
        next = strchr(start, '\\');
        if (next == NULL)
        {
            Logger_dbg("set all ");
            stringBuffer_append(stringBuffer, start);
            break;
        }
        
        //ヒットした場合はそこまでの文字列を生成して連結
        long length = next - start;
        char* tmp = Memory_malloc(length + 1);
        memset(tmp, 0x00, length + 1);
        strncpy(tmp, start, length);
        stringBuffer_append(stringBuffer, tmp);
        
        switch (*(next + 1))
        {
        case 'n':
            stringBuffer_appendChar(stringBuffer, '\n');
            break;
        case '"':
            stringBuffer_appendChar(stringBuffer, '"');
            break;
        default:
            stringBuffer_appendChar(stringBuffer, *(next + 1));
            break;
        }
        
        start = next + 2;
    }
    
    
    //最後の'"'を消去
    String value = stringBuffer_toString(stringBuffer);
    stringBuffer_dispose(stringBuffer);
    value->pointer[strlen(value->pointer) - 1] = '\0';
    Logger_dbg("Edited string is '%s'", value->pointer);
    
    
    string = CriaString_new(NULL, TRUE, value);
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)string;
}



List
evaluator_evaluateParametersExpression(
    Interpreter             interpreter,
    List                    local,
    ParametersExpression    parametersExpression
)
{
    Logger_trc("[ START ]%s", __func__);
    List expressions = parametersExpression->list;
    Expression expression = NULL;
    Item item = expressions->item;
    
    List list = list_new();
    CriaId id = NULL;
    
    while (item != NULL)
    {
        expression = (Expression)(item->object);
        switch (expression->kind)
        {
        case EXPRESSION_KIND_STRING_LITERAL:
            Logger_dbg("Do 'String literal expression'");
            id = (CriaId)evaluator_evaluateStringLiteralExpression(interpreter, expression->of._stringLiteral_);
            Logger_dbg("Done 'String literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        /*
        case EXPRESSION_KIND_INTEGER_LITERAL:
            break;
        case EXPRESSION_KIND_REAL_LITERAL:
            break;
        case EXPRESSION_KIND_BOOLEAN_LITERAL:
            break;
        case EXPRESSION_KIND_NULL_LITERAL:
            break;
        case EXPRESSION_KIND_GENERATE:
            break;
        //*/
        case EXPRESSION_KIND_FUNCTION_CALL:
            Logger_dbg("Do 'Function call expression'");
            id = evaluator_evaluateFunctionCallExpression(interpreter, local, expression->of._functionCall_);
            Logger_dbg("Done 'Function call expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        /*
        case EXPRESSION_KIND_VARIABLE:
            break;
        case EXPRESSION_KIND_OPERATION:
            break;
        //*/
        default:
            break;
        }
        item = item->next;
    }
    
    
    Logger_trc("[  END  ]%s", __func__);
    return list;
}



CriaId
evaluator_evaluateFunctionCallExpression(
    Interpreter             interpreter,
    List                    local,
    FunctionCallExpression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    FunctionDefinition  function;
    
    
    Logger_dbg("Function name is '%s'", expression->name->pointer);
    function = Interpreter_searchFunction(interpreter, expression->name->pointer);
    if (function == NULL)
    {
        Logger_dbg("Function is not found.");
        perror("Function is not found.\n");
        goto END;
    }
    
    
    //引数の式を実行
    List parameters = evaluator_evaluateParametersExpression(interpreter, local, expression->parameters);
    
    Logger_dbg("execute parameters count is '%d'", parameters->count);
    
    
    if (function->isNative == TRUE)
    {
        Logger_dbg("Call native function.(%s)", expression->name->pointer);
        id = (*(function->of.native.function))(interpreter, parameters);
    }
    else
    {
        Logger_dbg("Call cria function.(%s)", expression->name);
        //object = interpreter_callCriaFunction(interpreter, local, expression, function);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
evaluator_evaluateExpression(
    Interpreter             interpreter,
    List                    local,
    Expression              expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    FunctionDefinition  function;
    
    
    Logger_dbg("Function name is '%s'", expression->name->pointer);
    function = Interpreter_searchFunction(interpreter, expression->name->pointer);
    if (function == NULL)
    {
        Logger_dbg("Function is not found.");
        perror("Function is not found.\n");
        goto END;
    }
    
    
    //引数の式を実行
    List parameters = evaluator_evaluateParametersExpression(interpreter, local, expression->parameters);
    
    Logger_dbg("execute parameters count is '%d'", parameters->count);
    
    
    if (function->isNative == TRUE)
    {
        Logger_dbg("Call native function.(%s)", expression->name->pointer);
        id = (*(function->of.native.function))(interpreter, parameters);
    }
    else
    {
        Logger_dbg("Call cria function.(%s)", expression->name);
        //object = interpreter_callCriaFunction(interpreter, local, expression, function);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



