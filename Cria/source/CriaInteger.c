#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "CriaBoolean.h"
#include "CriaString.h"
#include "Runtime.h"
#include "CriaInteger.h"



CriaInteger
CriaInteger_new(
    Boolean isLiteral,
    int     value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaInteger integer = Memory_malloc(sizeof(struct CriaIntegerTag));
    
    integer->id.name = String_new("Integer");
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
        id = (CriaId)CriaInteger_new(FALSE, leftValue + rightValue);
        break;
    case OPERATION_KIND_MINUS:
        id = (CriaId)CriaInteger_new(FALSE, leftValue - rightValue);
        break;
    case OPERATION_KIND_MULTIPLY:
        id = (CriaId)CriaInteger_new(FALSE, leftValue * rightValue);
        break;
    case OPERATION_KIND_DIVIDE:
        id = (CriaId)CriaInteger_new(FALSE, leftValue / rightValue);
        break;
    case OPERATION_KIND_MODULO:
        id = (CriaId)CriaInteger_new(FALSE, leftValue % rightValue);
        break;
    case OPERATION_KIND_EQUAL:
        if (leftValue == rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    case OPERATION_KIND_LESS_THAN:
        if (leftValue < rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    case OPERATION_KIND_LESS_EQUAL:
        if (leftValue <= rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (leftValue != rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    default:
    	Runtime_error(interpreter, "Not supported operation.");
        break;
    }
    
    return id;
}



int
CriaInteger__core_(
	Interpreter interpreter,
	CriaId object
)
{
    Logger_trc("[ START ]%s", __func__);
    int value = 0;

    
    Logger_dbg("Check data type.");
    if (object->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Runtime_error(interpreter, "Scoped object data type is not Integer.");
    	goto END;
    }

    if (strcmp(object->name, "Integer") != 0)
    {
    	Runtime_error(interpreter, "Scoped object data type name is not Integer.");
    	goto END;
    }

    value = ((CriaInteger)object)->value;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



CriaId
CriaInteger_parse(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    int value = 0;
    CriaId arg = NULL;
    String string = NULL;
    long length = 0;
    int index = 0;
    char c = '\0';

    
    //value = CriaInteger__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	Runtime_error(interpreter, "Illegal argument count.");
    	goto END;
    }
    
    
    arg = (CriaId)(List_get(args, 0));
    if (arg->type != CRIA_DATA_TYPE_STRING)
    {
    	Runtime_error(interpreter, "1st data type is not String.");
    	goto END;
    }
    
    
    string = ((CriaString)arg)->value;
    length = strlen(string);
    for (index = 0; index < length; index++)
    {
        c = string[index];
        if (isdigit(c) == 0)
        {
        	Runtime_error(interpreter, "Illegal charactor '%c' in target string.", c);
        	goto END;
        }
    }
    
    
    value = atoi(string);
    id = (CriaId)CriaInteger_new(FALSE, value);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaInteger_to_string(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    int value = 0;
    char buffer[64];
    String string = NULL;

    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	Runtime_error(interpreter, "Illegal argument count.");
    	goto END;
    }
    
    
    value = CriaInteger__core_(interpreter, object);
    memset(buffer, 0x00, sizeof(buffer));
    sprintf(buffer, "%d", value);
    
    string = String_new(buffer);
    
    id = (CriaId)CriaString_new(FALSE, string);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



DefinitionClass
CriaInteger_loadClass(
    String className
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    Hash i_fields = Hash_new(32);
    Hash s_fields = Hash_new(32);
    Hash i_methods = Hash_new(32);
    Hash s_methods = Hash_new(32);
    
    function = DefinitionFunction_new("parse", TRUE, TRUE, NULL, NULL, CriaInteger_parse);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("to_string", TRUE, FALSE, NULL, NULL, CriaInteger_to_string);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}

