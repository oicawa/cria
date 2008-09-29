#include <stdio.h>

#include "_Cria.h"



CriaString*
evaluator_evaluateStringLiteralExpression(
    Interpreter             interpreter,
    StringLiteralExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString* string = NULL;
    string = CriaString_new(NULL, TRUE, string_clone(expression->value));
    Logger_trc("[  END  ]%s", __func__);
    return string;
}



List
evaluator_evaluateParametersExpression(
    Interpreter             interpreter,
    ParametersExpression    parametersExpression
)
{
    Logger_trc("[ START ]%s", __func__);
    List expressions = parametersExpression->list;
    Expression expression = NULL;
    Item item = expressions->item;
    
    List list = list_new();
    CriaString* string = NULL;
    
    while (item != NULL)
    {
        expression = (Expression)(item->object);
        switch (expression->kind)
        {
        case EXPRESSION_KIND_STRING_LITERAL:
            Logger_dbg("Do 'String literal expression'");
            string = evaluator_evaluateStringLiteralExpression(interpreter, expression->of._stringLiteral_);
            Logger_dbg("Done 'String literal expression'");
            list_add(list, string);
            Logger_dbg("Add 'Cria String'");
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



CriaObject
evaluator_evaluateFunctionCallExpression(
    Interpreter             interpreter,
    List                    local,
    FunctionCallExpression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject          object;
    FunctionDefinition  function;
    
    
    function = Interpreter_searchFunction(interpreter, expression->name->pointer);
    if (function == NULL)
    {
        Logger_dbg("Function is not found.");
        perror("Function is not found.\n");
        goto END;
    }
    
    
    //ˆø”‚ÌŽ®‚ðŽÀs
    List parameters = evaluator_evaluateParametersExpression(interpreter, expression->parameters);
    
    Logger_dbg("execute parameters count is '%d'", parameters->count);
    
    
    if (function->isNative == TRUE)
    {
        Logger_dbg("Call native function.(%s)", expression->name->pointer);
        object = (*(function->of.native.function))(interpreter, parameters);
    }
    else
    {
        Logger_dbg("Call cria function.(%s)", expression->name);
        //object = interpreter_callCriaFunction(interpreter, local, expression, function);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return object;
}



