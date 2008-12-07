#include <string.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_StringBuffer.h"
#include "_CriaId.h"
#include "_CriaInteger.h"
#include "_CriaBoolean.h"
#include "_CriaString.h"
#include "_Interpreter.h"
#include "_Expression.h"
#include "_DefinitionVariable.h"
#include "_DefinitionFunction.h"
#include "_DefinitionClass.h"
#include "_Runtime.h"

#include "_Evaluator.h"



DefinitionVariable
evaluator_referenceVariable(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ReferenceVariable   variable
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    
    if (parameters != NULL)
    {
        definition = definition_variable_search(parameters, variable->name);
        if (definition != NULL)
        {
            goto END;
        }
    }
    
    
    definition = definition_variable_search(interpreter->variables, variable->name);
    if (definition != NULL)
    {
        goto END;
    }
    
    //存在しなかった場合は登録
    if (parameters != NULL)
    {
        definition = definition_variable_new(variable->name);
        list_add(parameters, definition);
        goto END;
    }
    
    definition = definition_variable_new(variable->name);
    list_add(interpreter->variables, definition);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



DefinitionVariable
evaluator_reference(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    Reference   reference
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    switch (reference->type)
    {
    //case REFERENCE_TYPE_SELF:
    //    break;
    case REFERENCE_TYPE_VARIABLE:
        definition = evaluator_referenceVariable(interpreter, object, parameters, reference->of.variable);
        break;
    //case REFERENCE_TYPE_FUNCTION_CALL:
    //    definition = evaluator_referenceFunctionCall(interpreter, reference->of.function);
    //    break;
    //case REFERENCE_TYPE_CLASS:
    //    break;
    default:
        break;
    }
    

    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



