#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "_Reference.h"



Reference
Reference_new(
    ReferenceType   type
)
{
    Reference reference = NULL;
    
    reference = Memory_malloc(sizeof(struct ReferenceTag));
    memset(reference, 0x00, sizeof(struct ReferenceTag));
    reference->type = type;
    
    return reference;
}



DefinitionVariable
ReferenceVariable_evaluate(
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
    
    
    definition = definition_variable_search(Interpreter_variables(interpreter), variable->name);
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
    list_add(Interpreter_variables(interpreter), definition);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



DefinitionVariable
Reference_evaluate(
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
        definition = ReferenceVariable_evaluate(interpreter, object, parameters, reference->of.variable);
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



