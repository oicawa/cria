#include <stdio.h>

#include "_Cria.h"


Object
evaluator_evaluateFunctionCallExpression(
    Interpreter             interpreter,
    LocalEnvironment        local,
    FunctionCallExpression  expression
)
{
    Object              object;
    FunctionDefinition  function;
    
    function = interpreter_searchFunction(expression->name);
    if (function == NULL) {
        crb_runtime_error(expr->line_number, FUNCTION_NOT_FOUND_ERR,
                          STRING_MESSAGE_ARGUMENT, "name", identifier,
                          MESSAGE_ARGUMENT_END);
    }
    switch (function->type)
    {
    case FUNCTION_TYPE_CRIA:
        object = interpreter_callCriaFunction(interpreter, local, expression, function);
        break;
    case FUNCTION_TYPE_NATIVE:
        object = interpreter_callNativeFunction(interpreter, local, expression, function->u.native_f.proc);
        break;
    default:
        DBG_assert(0, ("bad case..%d\n", func->type));
    }

    return object;
}

