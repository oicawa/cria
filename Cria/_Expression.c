#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "String.h"
#include "StringBuffer.h"
#include "CriaBoolean.h"
#include "CriaString.h"
#include "CriaInteger.h"
#include "DefinitionVariable.h"
#include "DefinitionClass.h"
#include "DefinitionFunction.h"
#include "Runtime.h"

#include "_Expression.h"


Expression
expression_new(
    ExpressionKind  kind
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = Memory_malloc(sizeof(struct ExpressionTag));
    memset(expression, 0x00, sizeof(struct ExpressionTag));
    expression->kind = kind;
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



void
expression_dispose(
    Expression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    if (expression == NULL)
    {
        goto END;
    }
    
    switch (expression->kind)
    {
    case EXPRESSION_KIND_OPERATION:
        //operationExpression_dispose(expression->of._operation_);
        break;
    case EXPRESSION_KIND_STRING_LITERAL:
        break;
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
    case EXPRESSION_KIND_FUNCTION_CALL:
        break;
    case EXPRESSION_KIND_VARIABLE:
        break;
    default:
        Logger_err("Illegal statement type. (%d)", expression->kind);
        break;
    }
    Memory_free(expression);
    expression = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



CriaId
ExpressionOperation_evaluate(
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
    CriaId left = Expression_evaluate(interpreter, object, parameters, expression->left);
    Logger_dbg("Evaluated left expression.");
    CriaId right = Expression_evaluate(interpreter, object, parameters, expression->right);
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



CriaId
ExpressionVariable_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameterList,
    ExpressionVariable  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    DefinitionVariable variable;
    
    
    Logger_dbg("Variable name is '%s'", expression->name);
    
    
    if (parameterList != NULL)
    {
        variable = definition_variable_search(parameterList, expression->name);
        if (variable != NULL)
        {
            id = DefinitionVariable_getObject(variable);
            goto END;
        }
    }
    
    
    variable = definition_variable_search(interpreter->variables, expression->name);
    if (variable != NULL)
    {
        id = DefinitionVariable_getObject(variable);
        goto END;
    }
    
    Logger_err("Variable is not found.");
    runtime_error(interpreter);
    
END:
	Logger_dbg("variable->object = [%p]", DefinitionVariable_getObject(variable));
    Logger_trc("[  END  ]%s", __func__);
    return DefinitionVariable_getObject(variable);
}



CriaId
ExpressionReference_evaluate(
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
        id = ExpressionVariable_evaluate(interpreter, variable, parameters, expression->of.variable);
        break;
    case REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL:
        id = ExpressionFunctionCall_evaluate(interpreter, variable, parameters, expression->of.function);
        break;
    case REFERENCE_EXPRESSION_TYPE_CLASS:
        break;
    case REFERENCE_EXPRESSION_TYPE_GENERATE:
        id = ExpressionGenerate_evaluate(interpreter, variable, parameters, expression->of.generate);
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
    id = ExpressionReference_evaluate(interpreter, id, parameters, expression->next);

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



List
ExpressionParameters_evaluate(
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
            id = (CriaId)ExpressionStringLiteral_evaluate(interpreter, object, parameterList, expression->of._stringLiteral_);
            Logger_dbg("Done 'String literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_INTEGER_LITERAL:
            Logger_dbg("Do 'Integer literal expression'");
            id = ExpressionIntegerLiteral_evaluate(interpreter, object, parameterList, expression->of._integerLiteral_);
            Logger_dbg("Done 'Integer literal expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_BOOLEAN_LITERAL:
            Logger_dbg("Do 'Boolean literal expression'");
            id = ExpressionBooleanLiteral_evaluate(interpreter, object, parameterList, expression->of._booleanLiteral_);
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
            id = ExpressionFunctionCall_evaluate(interpreter, object, parameterList, expression->of._functionCall_);
            Logger_dbg("Done 'Function call expression'");
            list_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_REFERENCE:
            Logger_dbg("Do reference expression");
            id = ExpressionReference_evaluate(interpreter, object, parameterList, expression->of._reference_);
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
            id = ExpressionOperation_evaluate(interpreter, object, parameterList, expression->of._operation_);
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
ExpressionFunctionCall_evaluate(
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
		Logger_dbg("Method name is '%s'", expression->name);
		
		klass = definition_class_search(interpreter->classes, object->name);
		if (klass == NULL)
		{
			runtime_error(interpreter);
		}
		function = definition_function_search(DefinitionClass_getMethods(klass), expression->name);
		if (function == NULL)
		{
			Logger_dbg("Method is not found.");
			runtime_error(interpreter);
			goto END;
		}
		Logger_dbg("Method matched.");
    }
    else
    {
		Logger_dbg("Function name is '%s'", expression->name);
		function = definition_function_search(interpreter->functions, expression->name);
		if (function == NULL)
		{
			Logger_dbg("Function is not found.");
			runtime_error(interpreter);
			goto END;
		}
		Logger_dbg("Function matched.");
    }
    
    
    
    //引数の式を実行
    Logger_dbg("expression->name->pointer = %s", expression->name);
    Logger_dbg("expression->parameters->list->count = %d", expression->parameters->list->count);
    List parameters = ExpressionParameters_evaluate(interpreter, object, parameterList, expression->parameters);
    Logger_dbg("execute parameters count is '%d'", parameters->count);
    
    
    //if (function->isNative == TRUE)
    //{
        //Logger_dbg("Call native function.(%s)", expression->name);
        //id = (*(function->of.native.function))(interpreter, object, parameters);
        //goto END;
    //}
    
    
    Logger_dbg("Call cria function.(%s)", expression->name);
    id = definition_function_evaluate(interpreter, object, DefinitionFunction_getParameterList(function), function, parameters);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
ExpressionBooleanLiteral_evaluate(
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
ExpressionGenerate_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameterList,
    ExpressionGenerate  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    DefinitionClass  klass;
    
    
    Logger_dbg("Class name is '%s'", expression->name);
    klass = definition_class_search(interpreter->classes, expression->name);
    if (klass == NULL)
    {
        Logger_dbg("Class is not found.");
        runtime_error(interpreter);
        goto END;
    }
    Logger_dbg("Class matched.");
    
    
    //引数の式を実行
    List parameters = ExpressionParameters_evaluate(interpreter, object, parameterList, expression->parameters);
    
    
    id = definition_class_evaluate(interpreter, NULL, parameterList, "new", klass, parameters);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
ExpressionIntegerLiteral_evaluate(
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
ExpressionStringLiteral_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionStringLiteral expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaString string = NULL;
    //long length = string_length(expression->value);
    char* buffer = expression->value;
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
    value[strlen(value) - 1] = '\0';
    Logger_dbg("Edited string is '%s'", value);
    
    
    string = CriaString_new(TRUE, value);
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)string;
}



CriaId
Expression_evaluate(
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
        id = (CriaId)ExpressionStringLiteral_evaluate(interpreter, object, parameters, expression->of._stringLiteral_);
        Logger_dbg("Done 'String literal expression'");
        break;
    case EXPRESSION_KIND_INTEGER_LITERAL:
        Logger_dbg("Do 'Integer literal expression'");
        id = ExpressionIntegerLiteral_evaluate(interpreter, object, parameters, expression->of._integerLiteral_);
        Logger_dbg("Done 'Integer literal expression'");
        break;
    case EXPRESSION_KIND_GENERATE:
        Logger_dbg("Do 'Generate expression'");
        id = ExpressionGenerate_evaluate(interpreter, object, parameters, expression->of._generate_);
        Logger_dbg("Done 'Generate  expression'");
        break;
    /*
    case EXPRESSION_KIND_REAL_LITERAL:
        break;
    //*/
    case EXPRESSION_KIND_BOOLEAN_LITERAL:
        id = ExpressionBooleanLiteral_evaluate(interpreter, object, parameters, expression->of._booleanLiteral_);
        break;
    /*
    case EXPRESSION_KIND_NULL_LITERAL:
        break;
    //*/
    case EXPRESSION_KIND_FUNCTION_CALL:
        Logger_dbg("Do 'Function call expression'");
        id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, expression->of._functionCall_);
        Logger_dbg("Done 'Function call expression'");
        break;
    case EXPRESSION_KIND_REFERENCE:
        Logger_dbg("Do reference expression");
        id = ExpressionReference_evaluate(interpreter, object, parameters, expression->of._reference_);
        Logger_dbg("Done reference expression");
        break;
    case EXPRESSION_KIND_VARIABLE:
        Logger_dbg("Do variable expression");
        id = ExpressionVariable_evaluate(interpreter, object, parameters, expression->of._variable_);
        Logger_dbg("Done variable expression");
        break;
    case EXPRESSION_KIND_OPERATION:
        Logger_dbg("Do operation expression");
        id = ExpressionOperation_evaluate(interpreter, object, parameters, expression->of._operation_);
        Logger_dbg("Done operation expression");
        break;
    default:
        runtime_error(interpreter);
        break;
    }

    
    
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



