#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "CriaBoolean.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"

#include "_CriaString.h"


CriaString
CriaString_new(
    Boolean         isLiteral,
    String          value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString criaString = Memory_malloc(sizeof(struct CriaStringTag));
    memset(criaString, 0x00, sizeof(struct CriaStringTag));
    
    criaString->id.name = String_new("String");
    criaString->id.type = CRIA_DATA_TYPE_STRING;
    criaString->isLiteral = isLiteral;
    criaString->value = String_clone(value);
    
    Logger_trc("[  END  ]%s", __func__);
    return criaString;
}



void
CriaString_dispose(
    CriaString criaString
)
{
    Logger_trc("[ START ]%s", __func__);
    
    String_dispose(criaString->id.name);
    String_dispose(criaString->value);
    
    Memory_free(criaString);
    
    Logger_trc("[  END  ]%s", __func__);
}



CriaId
CriaString_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaString      left,
    CriaString      right
)
{
    String leftValue = left->value;
    String rightValue = right->value;
    StringBuffer buffer = NULL;
    Boolean result = FALSE;
    CriaId id = NULL;
    
    switch (kind)
    {
    case OPERATION_KIND_PLUS:
        buffer = StringBuffer_new();
        StringBuffer_append(buffer, leftValue);
        StringBuffer_append(buffer, rightValue);
        id = (CriaId)CriaString_new(FALSE, StringBuffer_toString(buffer));
        StringBuffer_dispose(buffer);
        break;
    case OPERATION_KIND_EQUAL:
        if (strcmp(leftValue, rightValue) == 0)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (strcmp(leftValue, rightValue) != 0)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    default:
        runtime_error(interpreter);
        break;
    }
    
    
    return id;
}



