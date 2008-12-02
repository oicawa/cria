#include <stdio.h>

#include "_Cria.h"



CriaId
evaluator_integerLiteral(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionIntegerLiteral expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaInteger integer = NULL;
    int value = expression->value;
    
    integer = CriaInteger_new(TRUE, value);

    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)integer;
}



CriaId
evaluator_booleanLiteral(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionBooleanLiteral expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBoolean boolean = NULL;
    Boolean value = expression->value;
    
    boolean = CriaBoolean_new(TRUE, value);

    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)boolean;
}



CriaId
evaluator_stringLiteral(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionStringLiteral expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString string = NULL;
    //long length = string_length(expression->value);
    char* buffer = expression->value->pointer;
    char* start = &buffer[1];   //先頭の'"'は読み飛ばす
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
        
        //ヒットした場合はそこまでの文字列を生成して連結
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
    
    
    //最後の'"'を消去
    String value = stringBuffer_toString(stringBuffer);
    stringBuffer_dispose(stringBuffer);
    value->pointer[strlen(value->pointer) - 1] = '\0';
    Logger_dbg("Edited string is '%s'", value->pointer);
    
    
    string = CriaString_new(TRUE, value);
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)string;
}



CriaId
evaluator_variable(
    Interpreter         interpreter,
    CriaId object,
    List parameterList,
    ExpressionVariable  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    DefinitionVariable variable;
    
    
    Logger_dbg("Variable name is '%s'", expression->name->pointer);
    
    
    if (parameterList != NULL)
    {
        variable = variableDefinition_search(parameterList, expression->name);
        if (variable != NULL)
        {
            id = variable->object;
            goto END;
        }
    }
    
    
    variable = variableDefinition_search(interpreter->variables, expression->name);
    if (variable != NULL)
    {
        id = variable->object;
        goto END;
    }
    
    Logger_err("Variable is not found.");
    runtime_error(interpreter);
    
END:
	Logger_dbg("variable->object = [%p]", variable->object);
    Logger_trc("[  END  ]%s", __func__);
    return variable->object;
}



CriaId
evaluator_operation(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ExpressionOperation expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    if (expression->left == NULL)
    {
        Logger_dbg("expression->left is NULL!");
    }
    CriaId left = evaluator_expression(interpreter, object, parameters, expression->left);
    Logger_dbg("Evaluated left expression.");
    CriaId right = evaluator_expression(interpreter, object, parameters, expression->right);
    Logger_dbg("Evaluated right expression.");
    
    if (left == NULL)
    {
        Logger_dbg("Left is NULL!");
    }
    Logger_dbg("Check data type.");
    Logger_dbg("Data type = %d. (INTEGER is %d)", left->type, CRIA_DATA_TYPE_INTEGER);
    Logger_dbg("Checked data type.");
    
    //式の左辺右辺の型チェック
    if (left->type != right->type)
    {
        runtime_error(interpreter);
    }
    
    //演算可能・不可能のチェック
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
    CriaId object,
    List parameterList,
    ExpressionParameters    parametersExpression
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
            id = (CriaId)evaluator_stringLiteral(interpreter, object, parameterList, expression->of._stringLiteral_);
            Logger_dbg("Done 'String literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_INTEGER_LITERAL:
            Logger_dbg("Do 'Integer literal expression'");
            id = evaluator_integerLiteral(interpreter, object, parameterList, expression->of._integerLiteral_);
            Logger_dbg("Done 'Integer literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_BOOLEAN_LITERAL:
            Logger_dbg("Do 'Boolean literal expression'");
            id = evaluator_booleanLiteral(interpreter, object, parameterList, expression->of._booleanLiteral_);
            Logger_dbg("Done 'Boolean literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        /*
        case EXPRESSION_KIND_REAL_LITERAL:
            break;
        case EXPRESSION_KIND_NULL_LITERAL:
            break;
        case EXPRESSION_KIND_GENERATE:
            break;
        //*/
        case EXPRESSION_KIND_FUNCTION_CALL:
            Logger_dbg("Do 'Function call expression'");
            id = evaluator_functionCall(interpreter, object, parameterList, expression->of._functionCall_);
            Logger_dbg("Done 'Function call expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_REFERENCE:
            Logger_dbg("Do reference expression");
            id = evaluator_referenceExpression(interpreter, object, parameterList, expression->of._reference_);
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
            id = evaluator_operation(interpreter, object, parameterList, expression->of._operation_);
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
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionFunctionCall expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    DefinitionFunction  function = NULL;
    DefinitionClass klass = NULL;
    
    
    if (object != NULL)
    {
		Logger_dbg("Method name is '%s'", expression->name->pointer);
		
		klass = classDefinition_search(interpreter->classes, object->name->pointer);
		if (klass == NULL)
		{
			runtime_error(interpreter);
		}
		function = functionDefinition_search(klass->methodList, expression->name->pointer);
		if (function == NULL)
		{
			Logger_dbg("Method is not found.");
			runtime_error(interpreter);
			goto END;
		}
		Logger_dbg("Method matched. (%s)", function->name);
    }
    else
    {
		Logger_dbg("Function name is '%s'", expression->name->pointer);
		function = functionDefinition_search(interpreter->functions, expression->name->pointer);
		if (function == NULL)
		{
			Logger_dbg("Function is not found.");
			runtime_error(interpreter);
			goto END;
		}
		Logger_dbg("Function matched. (%s)", function->name);
    }
    
    
    
    //引数の式を実行
    Logger_dbg("expression->name->pointer = %s", expression->name->pointer);
    Logger_dbg("expression->parameters->list->count = %d", expression->parameters->list->count);
    List parameters = evaluator_parameters(interpreter, object, parameterList, expression->parameters);
    Logger_dbg("execute parameters count is '%d'", parameters->count);
    
    
    if (function->isNative == TRUE)
    {
        Logger_dbg("Call native function.(%s)", expression->name->pointer);
        id = (*(function->of.native.function))(interpreter, object, parameters);
        goto END;
    }
    
    
    Logger_dbg("Call cria function.(%s)", expression->name->pointer);
    id = functionDefinition_evaluate(interpreter, object, function->of.cria.parameterList, function, parameters);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
evaluator_generate(
    Interpreter             interpreter,
    CriaId object,
    List parameterList,
    ExpressionGenerate  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    DefinitionClass  klass;
    
    
    Logger_dbg("Class name is '%s'", expression->name->pointer);
    klass = classDefinition_search(interpreter->classes, expression->name->pointer);
    if (klass == NULL)
    {
        Logger_dbg("Class is not found.");
        runtime_error(interpreter);
        goto END;
    }
    Logger_dbg("Class matched. (%s)", klass->name->pointer);
    
    
    //引数の式を実行
    Logger_dbg("expression->name->pointer = %s", expression->name->pointer);
    Logger_dbg("expression->parameters->list->count = %d", expression->parameters->list->count);
    List parameters = evaluator_parameters(interpreter, object, parameterList, expression->parameters);
    Logger_dbg("execute parameters count is '%d'", parameters->count);
    
    
    Logger_dbg("Call cria constractor.(%s)", expression->name->pointer);
    id = classDefinition_evaluate(interpreter, NULL, parameterList, "new", klass, parameters);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



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
        definition = variableDefinition_search(parameters, variable->name);
        if (definition != NULL)
        {
            goto END;
        }
    }
    
    
    definition = variableDefinition_search(interpreter->variables, variable->name);
    if (definition != NULL)
    {
        goto END;
    }
    
    //存在しなかった場合は登録
    if (parameters != NULL)
    {
        definition = variableDefinition_new(variable->name);
        list_add(parameters, definition);
        goto END;
    }
    
    definition = variableDefinition_new(variable->name);
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



CriaId
evaluator_referenceExpression(
    Interpreter         interpreter,
    CriaId variable,
    List parameters,
    ExpressionReference expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    switch (expression->type)
    {
    case REFERENCE_EXPRESSION_TYPE_SELF:
        break;
    case REFERENCE_EXPRESSION_TYPE_VARIABLE:
        id = evaluator_variable(interpreter, variable, parameters, expression->of.variable);
        break;
    case REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL:
        id = evaluator_functionCall(interpreter, variable, parameters, expression->of.function);
        break;
    case REFERENCE_EXPRESSION_TYPE_CLASS:
        break;
    case REFERENCE_EXPRESSION_TYPE_GENERATE:
        id = evaluator_generate(interpreter, variable, parameters, expression->of.generate);
        break;
    default:
        break;
    }
    
    if (expression->next == NULL)
    {
    	Logger_dbg("Next ReferenceExpression does not exist.");
    	goto END;
    }
    
    Logger_dbg("Evaluate next ReferenceExpression.");
    id = evaluator_referenceExpression(interpreter, id, parameters, expression->next);

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
evaluator_expression(
    Interpreter interpreter,
    CriaId object,
	List parameters,
    Expression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    
    switch (expression->kind)
    {
    case EXPRESSION_KIND_STRING_LITERAL:
        Logger_dbg("Do 'String literal expression'");
        id = (CriaId)evaluator_stringLiteral(interpreter, object, parameters, expression->of._stringLiteral_);
        Logger_dbg("Done 'String literal expression'");
        break;
    case EXPRESSION_KIND_INTEGER_LITERAL:
        Logger_dbg("Do 'Integer literal expression'");
        id = evaluator_integerLiteral(interpreter, object, parameters, expression->of._integerLiteral_);
        Logger_dbg("Done 'Integer literal expression'");
        break;
    case EXPRESSION_KIND_GENERATE:
        Logger_dbg("Do 'Generate expression'");
        id = evaluator_generate(interpreter, object, parameters, expression->of._generate_);
        Logger_dbg("Done 'Generate  expression'");
        break;
    /*
    case EXPRESSION_KIND_REAL_LITERAL:
        break;
    //*/
    case EXPRESSION_KIND_BOOLEAN_LITERAL:
        id = evaluator_booleanLiteral(interpreter, object, parameters, expression->of._booleanLiteral_);
        break;
    /*
    case EXPRESSION_KIND_NULL_LITERAL:
        break;
    //*/
    case EXPRESSION_KIND_FUNCTION_CALL:
        Logger_dbg("Do 'Function call expression'");
        id = evaluator_functionCall(interpreter, object, parameters, expression->of._functionCall_);
        Logger_dbg("Done 'Function call expression'");
        break;
    case EXPRESSION_KIND_REFERENCE:
        Logger_dbg("Do reference expression");
        id = evaluator_referenceExpression(interpreter, object, parameters, expression->of._reference_);
        Logger_dbg("Done reference expression");
        break;
    case EXPRESSION_KIND_VARIABLE:
        Logger_dbg("Do variable expression");
        id = evaluator_variable(interpreter, object, parameters, expression->of._variable_);
        Logger_dbg("Done variable expression");
        break;
    case EXPRESSION_KIND_OPERATION:
        Logger_dbg("Do operation expression");
        id = evaluator_operation(interpreter, object, parameters, expression->of._operation_);
        Logger_dbg("Done operation expression");
        break;
    default:
        runtime_error(interpreter);
        break;
    }

    
    
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



