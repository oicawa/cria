#include <stdio.h>

#include "_Cria.h"



CriaId
evaluator_stringLiteral(
    Interpreter             interpreter,
    StringLiteralExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString string = NULL;
    //long length = string_length(expression->value);
    char* buffer = expression->value->pointer;
    char* start = &buffer[1];   //�擪��'"'�͓ǂݔ�΂�
    char* next = NULL;
    StringBuffer stringBuffer = stringBuffer_new();
    
    
    Logger_dbg("Literal is '%s'", buffer);
    
    while (*start != '\0')
    {
        Logger_dbg("literal check");
        next = strchr(start, '\\');
        if (next == NULL)
        {
            Logger_dbg("set all ");
            stringBuffer_append(stringBuffer, start);
            break;
        }
        
        //�q�b�g�����ꍇ�͂����܂ł̕�����𐶐����ĘA��
        long length = next - start;
        char* tmp = Memory_malloc(length + 1);
        memset(tmp, 0x00, length + 1);
        strncpy(tmp, start, length);
        stringBuffer_append(stringBuffer, tmp);
        
        switch (*(next + 1))
        {
        case 'n':
            stringBuffer_appendChar(stringBuffer, '\n');
            break;
        case '"':
            stringBuffer_appendChar(stringBuffer, '"');
            break;
        default:
            stringBuffer_appendChar(stringBuffer, *(next + 1));
            break;
        }
        
        start = next + 2;
    }
    
    
    //�Ō��'"'������
    String value = stringBuffer_toString(stringBuffer);
    stringBuffer_dispose(stringBuffer);
    value->pointer[strlen(value->pointer) - 1] = '\0';
    Logger_dbg("Edited string is '%s'", value->pointer);
    
    
    string = CriaString_new(NULL, TRUE, value);
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)string;
}



List
evaluator_parameters(
    Interpreter             interpreter,
    List                    local,
    ParametersExpression    parametersExpression
)
{
    Logger_trc("[ START ]%s", __func__);
    List expressions = parametersExpression->list;
    Expression expression = NULL;
    Item item = expressions->item;
    
    List list = list_new();
    CriaId id = NULL;
    
    while (item != NULL)
    {
        expression = (Expression)(item->object);
        switch (expression->kind)
        {
        case EXPRESSION_KIND_STRING_LITERAL:
            Logger_dbg("Do 'String literal expression'");
            id = (CriaId)evaluator_stringLiteral(interpreter, expression->of._stringLiteral_);
            Logger_dbg("Done 'String literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
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
            Logger_dbg("Do 'Function call expression'");
            id = evaluator_functionCall(interpreter, local, expression->of._functionCall_);
            Logger_dbg("Done 'Function call expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
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



CriaId
evaluator_functionCall(
    Interpreter             interpreter,
    List                    local,
    FunctionCallExpression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    FunctionDefinition  function;
    
    
    Logger_dbg("Function name is '%s'", expression->name->pointer);
    function = Interpreter_searchFunction(interpreter, expression->name->pointer);
    if (function == NULL)
    {
        Logger_dbg("Function is not found.");
        perror("Function is not found.\n");
        goto END;
    }
    
    
    //�����̎������s
    List parameters = evaluator_parameters(interpreter, local, expression->parameters);
    
    Logger_dbg("execute parameters count is '%d'", parameters->count);
    
    
    if (function->isNative == TRUE)
    {
        Logger_dbg("Call native function.(%s)", expression->name->pointer);
        id = (*(function->of.native.function))(interpreter, parameters);
    }
    else
    {
        Logger_dbg("Call cria function.(%s)", expression->name);
        //object = interpreter_callCriaFunction(interpreter, local, expression, function);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
evaluator_reference(
    Interpreter         interpreter,
    List                local,
    ReferenceExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    switch (expression->type)
    {
    case REFERENCE_TYPE_SELF:
        break;
    case REFERENCE_TYPE_VARIABLE:
        break;
    case REFERENCE_TYPE_FUNCTION_CALL:
        id = evaluator_functionCall(interpreter, local, expression->of.function);
        break;
    case REFERENCE_TYPE_CLASS:
        break;
    case REFERENCE_TYPE_GENERATE:
        break;
    default:
        break;
    }
    

    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
evaluator_expression(
    Interpreter             interpreter,
    List                    local,
    Expression              expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



