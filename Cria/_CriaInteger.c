#include "_Cria.h"



CriaInteger
CriaInteger_new(
    String  name,
    Boolean isLiteral,
    int     value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaInteger integer = Memory_malloc(sizeof(struct CriaIntegerTag));
    memset(integer, 0x00, sizeof(struct CriaIntegerTag));
    
    integer->id.name = string_clone(name);
    integer->id.type = CRIA_DATA_TYPE_INTEGER;
    integer->isLiteral = isLiteral;
    integer->value = value;
    
    Logger_trc("[  END  ]%s", __func__);
    return integer;
}



CriaId
CriaInteger_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaInteger     left,
    CriaInteger     right
)
{
    int leftValue = left->value;
    int rightValue = right->value;
    Boolean result = FALSE;
    CriaId id = NULL;
    
    switch (kind)
    {
    case OPERATION_KIND_PLUS:
        id = (CriaId)CriaInteger_new(NULL, FALSE, leftValue + rightValue);
        break;
    case OPERATION_KIND_MINUS:
        id = (CriaId)CriaInteger_new(NULL, FALSE, leftValue - rightValue);
        break;
    case OPERATION_KIND_MULTIPLY:
        id = (CriaId)CriaInteger_new(NULL, FALSE, leftValue * rightValue);
        break;
    case OPERATION_KIND_DIVIDE:
        id = (CriaId)CriaInteger_new(NULL, FALSE, leftValue / rightValue);
        break;
    case OPERATION_KIND_MODULO:
        id = (CriaId)CriaInteger_new(NULL, FALSE, leftValue % rightValue);
        break;
    case OPERATION_KIND_EQUAL:
        if (leftValue == rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(NULL, FALSE, result);
        break;
    case OPERATION_KIND_LESS_THAN:
        if (leftValue < rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(NULL, FALSE, result);
        break;
    case OPERATION_KIND_LESS_EQUAL:
        if (leftValue <= rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(NULL, FALSE, result);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (leftValue != rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(NULL, FALSE, result);
        break;
    default:
        runtime_error(interpreter);
        break;
    }
    
    return id;
}


