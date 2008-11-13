#include <stdio.h>

#include "_Cria.h"



CriaId
evaluator_integerLiteral(
    Interpreter             interpreter,
    IntegerLiteralExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaInteger integer = NULL;
    int value = expression->value;
    
    integer = CriaInteger_new(NULL, TRUE, value);

    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)integer;
}



CriaId
evaluator_booleanLiteral(
    Interpreter             interpreter,
    BooleanLiteralExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBoolean boolean = NULL;
    Boolean value = expression->value;
    
    boolean = CriaBoolean_new(NULL, TRUE, value);

    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)boolean;
}



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



CriaId
evaluator_variable(
    Interpreter         interpreter,
    VariableExpression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    VariableDefinition variable;
    
    
    Logger_dbg("Variable name is '%s'", expression->name->pointer);
    variable = variableDefinition_search(interpreter->variableList, expression->name);
    if (variable == NULL)
    {
        Logger_err("Variable is not found.");
        runtime_error(interpreter);
        goto END;
    }
    
    id = variable->object;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
evaluator_operation(
    Interpreter         interpreter,
    OperationExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    if (expression->left == NULL)
    {
        Logger_dbg("expression->left is NULL!");
    }
    CriaId left = evaluator_expression(interpreter, expression->left);
    Logger_dbg("Evaluated left expression.");
    CriaId right = evaluator_expression(interpreter, expression->right);
    Logger_dbg("Evaluated right expression.");
    
    if (left == NULL)
    {
        Logger_dbg("Left is NULL!");
    }
    Logger_dbg("Check data type.");
    Logger_dbg("Data type = %d. (INTEGER is %d)", left->type, CRIA_DATA_TYPE_INTEGER);
    Logger_dbg("Checked data type.");
    
    //���̍��ӉE�ӂ̌^�`�F�b�N
    if (left->type != right->type)
    {
        runtime_error(interpreter);
    }
    
    //���Z�\�E�s�\�̃`�F�b�N
    if (left->type == CRIA_DATA_TYPE_FUNCTION
        || left->type == CRIA_DATA_TYPE_CRIA_OBJECT
        || left->type == CRIA_DATA_TYPE_CRIA_FUNCTION)
    {
        runtime_error(interpreter);
    }
    
    switch (left->type)
    {
    case CRIA_DATA_TYPE_BOOLEAN:
        id = CriaBoolean_operate(interpreter, expression->kind, (CriaBoolean)left, (CriaBoolean)right);
        break;
    case CRIA_DATA_TYPE_INTEGER:
        Logger_dbg("Operate of integer.");
        id = CriaInteger_operate(interpreter, expression->kind, (CriaInteger)left, (CriaInteger)right);
        break;
    case CRIA_DATA_TYPE_STRING:
        id = CriaString_operate(interpreter, expression->kind, (CriaString)left, (CriaString)right);
        break;
    case CRIA_DATA_TYPE_NULL:
    case CRIA_DATA_TYPE_VOID:
    case CRIA_DATA_TYPE_LIST:
    case CRIA_DATA_TYPE_FUNCTION:
    case CRIA_DATA_TYPE_CRIA_OBJECT:
    case CRIA_DATA_TYPE_CRIA_FUNCTION:
    default:
        runtime_error(interpreter);
        break;
    }
    
    
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



List
evaluator_parameters(
    Interpreter             interpreter,
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
        case EXPRESSION_KIND_INTEGER_LITERAL:
            Logger_dbg("Do 'Integer literal expression'");
            id = evaluator_integerLiteral(interpreter, expression->of._integerLiteral_);
            Logger_dbg("Done 'Integer literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        /*
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
            id = evaluator_functionCall(interpreter, expression->of._functionCall_);
            Logger_dbg("Done 'Function call expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_REFERENCE:
            Logger_dbg("Do reference expression");
            id = evaluator_referenceExpression(interpreter, expression->of._reference_);
            Logger_dbg("Done reference expression");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        /*
        case EXPRESSION_KIND_VARIABLE:
            break;
        //*/
        case EXPRESSION_KIND_OPERATION:
            Logger_dbg("Do operation expression");
            id = evaluator_operation(interpreter, expression->of._operation_);
            Logger_dbg("Done operation expression");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
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
    FunctionCallExpression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    FunctionDefinition  function;
    
    
    Logger_dbg("Function name is '%s'", expression->name->pointer);
    function = functionDefinition_search(interpreter->functionList, expression->name->pointer);
    if (function == NULL)
    {
        Logger_dbg("Function is not found.");
        perror("Function is not found.\n");
        goto END;
    }
    
    
    //�����̎������s
    List parameters = evaluator_parameters(interpreter, expression->parameters);
    
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



VariableDefinition
evaluator_referenceVariable(
    Interpreter         interpreter,
    ReferenceVariable   variable
)
{
    Logger_trc("[ START ]%s", __func__);
    VariableDefinition definition = NULL;
    
    
    definition = variableDefinition_search(interpreter->variableList, variable->name);
    if (definition == NULL)
    {
        //���݂��Ȃ������ꍇ�͓o�^
        definition = variableDefinition_new(variable->name);
        list_add(interpreter->variableList, definition);
    }
    

    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



VariableDefinition
evaluator_reference(
    Interpreter interpreter,
    Reference   reference
)
{
    Logger_trc("[ START ]%s", __func__);
    VariableDefinition definition = NULL;
    
    switch (reference->type)
    {
    //case REFERENCE_TYPE_SELF:
    //    break;
    case REFERENCE_TYPE_VARIABLE:
        definition = evaluator_referenceVariable(interpreter, reference->of.variable);
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



CriaId
evaluator_referenceExpression(
    Interpreter         interpreter,
    ReferenceExpression expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    switch (expression->type)
    {
    case REFERENCE_EXPRESSION_TYPE_SELF:
        break;
    case REFERENCE_EXPRESSION_TYPE_VARIABLE:
        id = evaluator_variable(interpreter, expression->of.variable);
        break;
    case REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL:
        id = evaluator_functionCall(interpreter, expression->of.function);
        break;
    case REFERENCE_EXPRESSION_TYPE_CLASS:
        break;
    case REFERENCE_EXPRESSION_TYPE_GENERATE:
        break;
    default:
        break;
    }
    

    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
evaluator_expression(
    Interpreter interpreter,
    Expression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    
    switch (expression->kind)
    {
    case EXPRESSION_KIND_STRING_LITERAL:
        Logger_dbg("Do 'String literal expression'");
        id = (CriaId)evaluator_stringLiteral(interpreter, expression->of._stringLiteral_);
        Logger_dbg("Done 'String literal expression'");
        break;
    case EXPRESSION_KIND_INTEGER_LITERAL:
        Logger_dbg("Do 'Integer literal expression'");
        id = evaluator_integerLiteral(interpreter, expression->of._integerLiteral_);
        Logger_dbg("Done 'Integer literal expression'");
        break;
    /*
    case EXPRESSION_KIND_REAL_LITERAL:
        break;
    //*/
    case EXPRESSION_KIND_BOOLEAN_LITERAL:
        id = evaluator_booleanLiteral(interpreter, expression->of._booleanLiteral_);
        break;
    /*
    case EXPRESSION_KIND_NULL_LITERAL:
        break;
    case EXPRESSION_KIND_GENERATE:
        break;
    //*/
    case EXPRESSION_KIND_FUNCTION_CALL:
        Logger_dbg("Do 'Function call expression'");
        id = evaluator_functionCall(interpreter, expression->of._functionCall_);
        Logger_dbg("Done 'Function call expression'");
        break;
    case EXPRESSION_KIND_REFERENCE:
        Logger_dbg("Do reference expression");
        id = evaluator_referenceExpression(interpreter, expression->of._reference_);
        Logger_dbg("Done reference expression");
        break;
    case EXPRESSION_KIND_VARIABLE:
        Logger_dbg("Do variable expression");
        id = evaluator_variable(interpreter, expression->of._variable_);
        Logger_dbg("Done variable expression");
        break;
    case EXPRESSION_KIND_OPERATION:
        Logger_dbg("Do operation expression");
        id = evaluator_operation(interpreter, expression->of._operation_);
        Logger_dbg("Done operation expression");
        break;
    default:
        runtime_error(interpreter);
        break;
    }

    
    
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



