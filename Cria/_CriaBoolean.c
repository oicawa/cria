#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "Runtime.h"
#include "CriaString.h"
#include "String.h"

#include "_CriaBoolean.h"



CriaBoolean
CriaBoolean_new(
    Boolean isLiteral,
    Boolean value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBoolean boolean = Memory_malloc(sizeof(struct CriaBooleanTag));
    memset(boolean, 0x00, sizeof(struct CriaBooleanTag));
    
    char name[] = "Boolean";
    boolean->id.name = String_new(name);
    boolean->id.type = CRIA_DATA_TYPE_BOOLEAN;
    boolean->isLiteral = isLiteral;
    boolean->value = value;
    
    Logger_trc("[  END  ]%s", __func__);
    return boolean;
}



CriaId
CriaBoolean_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaBoolean     left,
    CriaBoolean     right
)
{
    Boolean leftValue = left->value;
    Boolean rightValue = right->value;
    Boolean result = FALSE;
    CriaId id = NULL;
    
    switch (kind)
    {
    case OPERATION_KIND_EQUAL:
        if (leftValue == rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (leftValue != rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    default:
        runtime_error(interpreter);
        break;
    }
    
    return id;
}



CriaId
CriaBoolean_toString(
    Interpreter     interpreter,
    CriaBoolean     boolean
)
{
    String value = NULL;
    CriaString string = NULL;
    
    if (boolean->value == TRUE)
        value = String_new("true");
    else if (boolean->value == FALSE)
        value = String_new("false");
    else
        runtime_error(interpreter);
    
    string = CriaString_new(FALSE, value);
    string_dispose(value);
    
    return (CriaId)string;
}



