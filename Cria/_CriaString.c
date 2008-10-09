#include "_Cria.h"



CriaString
CriaString_new(
    String          name,
    Boolean         isLiteral,
    String          value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString criaString = Memory_malloc(sizeof(struct CriaStringTag));
    memset(criaString, 0x00, sizeof(struct CriaStringTag));
    
    criaString->id.name = string_clone(name);
    criaString->id.type = CRIA_DATA_TYPE_STRING;
    criaString->isLiteral = isLiteral;
    criaString->value = string_clone(value);
    
    Logger_trc("[  END  ]%s", __func__);
    return criaString;
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
        buffer = stringBuffer_new();
        stringBuffer_append(buffer, leftValue->pointer);
        stringBuffer_append(buffer, rightValue->pointer);
        id = (CriaId)CriaString_new(NULL, FALSE, stringBuffer_toString(buffer));
        stringBuffer_dispose(buffer);
        break;
    case OPERATION_KIND_EQUAL:
        if (strcmp(leftValue->pointer, rightValue->pointer) == 0)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(NULL, FALSE, result);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (strcmp(leftValue->pointer, rightValue->pointer) != 0)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(NULL, FALSE, result);
        break;
    default:
        runtime_error(interpreter);
        break;
    }
    
    
    return id;
}



