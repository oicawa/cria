#include "_Cria.h"



CriaBoolean
CriaBoolean_new(
    String  name,
    Boolean isLiteral,
    Boolean value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBoolean boolean = Memory_malloc(sizeof(struct CriaBooleanTag));
    memset(boolean, 0x00, sizeof(struct CriaBooleanTag));
    
    boolean->id.name = string_clone(name);
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






