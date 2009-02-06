#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "StringBuffer.h"
#include "Runtime.h"
#include "CriaBoolean.h"
#include "CriaInteger.h"
#include "CriaString.h"
#include "CriaObject.h"
#include "CriaClass.h"
#include "Definition.h"
#include "Hash.h"

#include "_Expression.h"



ExpressionReference ExpressionFunctionCall_parse(Parser parser);
ExpressionReference ExpressionVariable_parse(Parser parser);
ExpressionReference ExpressionGenerate_parse(Parser parser);
ExpressionReference ExpressionClass_parse(Parser parser);
ExpressionParameters ExpressionParameters_parse(Parser parser);




Expression
Expression_new(
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
Expression_dispose(
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



//==============================
//ExpressionOperation
//==============================
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



//==============================
//ExpressionSelf
//==============================
ExpressionReference
ExpressionSelf_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    ExpressionReference expression = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
        Logger_dbg("Not self object.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    Logger_dbg("VariableExpression name = '_self_'");
    ExpressionVariable variable = NULL;
    variable = Memory_malloc(sizeof(struct ExpressionVariableTag));
    memset(variable, 0x00, sizeof(struct ExpressionVariableTag));
    variable->name = String_clone("_self_");
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_SELF;
    expression->of.variable = variable;

	Logger_dbg("Next ReferenceExprssion parse.");
	Parser_next(parser);
	expression->next = ExpressionReference_parse(parser);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



//==============================
//ExpressionVariable
//==============================
CriaId
ExpressionVariable_evaluateFromObject(
    Interpreter         interpreter,
    CriaId object,
    String variableName
)
{
    Logger_trc("[ START ]%s (Variable Name = '%s')", __func__, variableName);
    CriaId id = NULL;
    
    
    Logger_dbg("Object is '%p'", object);
    if (object == NULL)
    {
	    Logger_dbg("Object is NULL.");
	    goto END;
    }
    
    
	if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
	{
	    Logger_dbg("object->type = '%d'", object->type);
		goto END;
	}
	
	
	id = (CriaId)CriaObject_get(interpreter, (CriaObject)object, variableName);
	Logger_dbg("Found the target field. (%s)", variableName);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
ExpressionVariable_evaluateFromClass(
    Interpreter         interpreter,
    CriaId klass,
    String variableName
)
{
    Logger_trc("[ START ]%s (Variable Name = '%s')", __func__, variableName);
    CriaId id = NULL;
    
    
    Logger_dbg("Class is '%p'", klass);
    if (klass == NULL)
    {
	    Logger_dbg("Class is NULL.");
	    goto END;
    }
    
    
	if (klass->type != CRIA_DATA_TYPE_CRIA_CLASS)
	{
	    Logger_dbg("klass->type = '%d'", klass->type);
		goto END;
	}
	
	
	id = (CriaId)CriaClass_get(interpreter, (CriaClass)klass, variableName);
	Logger_dbg("Found the target field. (%s)", variableName);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
ExpressionVariable_evaluateFromParameters(
	Interpreter interpreter,
	List parameters,
	String variableName
)
{
    Logger_trc("[ START ]%s (Variable Name = '%s')", __func__, variableName);
    CriaId id = NULL;
    DefinitionVariable variable = NULL;
    
    
    Logger_dbg("Parameters is '%p'", parameters);
    if (parameters == NULL)
    {
	    Logger_dbg("Parameters is NULL.");
	    goto END;
    }
    
    
	variable = DefinitionVariable_search(parameters, variableName);
	if (variable == NULL)
	{
	    Logger_dbg("Definition variable not found.");
		goto END;
	}

	id = DefinitionVariable_getObject(variable);

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
ExpressionVariable_evaluateFromInterpreter(
	Interpreter interpreter,
	String variableName
)
{
    Logger_trc("[ START ]%s (Variable Name = '%s')", __func__, variableName);
    CriaId id = NULL;
    DefinitionVariable variable = NULL;
    
    
    variable = (DefinitionVariable)Hash_get(Interpreter_variables(interpreter), variableName);
    if (variable == NULL)
    {
	    Logger_dbg("Definition variable not found.");
        goto END;
    }
    
	id = DefinitionVariable_getObject(variable);

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
ExpressionVariable_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameterList,
    ExpressionVariable  expression,
    CriaId parent
)
{
    Logger_trc("[ START ]%s (variable name = '%s')", __func__, expression->name);
    CriaId id = NULL;
    
    
    Logger_dbg("parent is %p", parent);
    if (parent == NULL)
    {
		id = ExpressionVariable_evaluateFromParameters(interpreter, parameterList, expression->name);
		if (id != NULL)
			goto END;
		
		id = ExpressionVariable_evaluateFromInterpreter(interpreter, expression->name);
		if (id != NULL)
			goto END;
		
        runtime_error(interpreter);
        goto END;
    }
    else if (parent->type == CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	id = ExpressionVariable_evaluateFromObject(interpreter, parent, expression->name);
    	if (id != NULL)
    		goto END;
    }
    else if (parent->type == CRIA_DATA_TYPE_CRIA_CLASS)
    {
    	id = ExpressionVariable_evaluateFromClass(interpreter, parent, expression->name);
    	if (id != NULL)
    		goto END;
    }
	
	Logger_err("Field named '%s' is not found.", expression->name);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



//==============================
//ExpressionClass
//==============================
CriaId
ExpressionClass_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionClass klass,
    CriaId parent
)
{
    Logger_trc("[ START ]%s (class name = '%s')", __func__, klass->name);
    CriaId id = NULL;
    String className = NULL;
    DefinitionClass definition = NULL;
    Hash classes = NULL;
    
    if (strcmp(klass->name, "class") == 0)
    {
		className = object->name;
    }
    else
    {
		className = klass->name;
    }
    Logger_dbg("Real class name = '%s'.", className);
    
    
    classes = Interpreter_classes(interpreter);
    definition = (DefinitionClass)Hash_get(classes, className);
    if (definition == NULL)
    {
        Logger_err("Specified class not found. ('%s')", className);
        runtime_error(interpreter);
        goto END;
    }
    
    id = (CriaId)CriaClass_new(definition);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



ExpressionReference
ExpressionClass_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    ExpressionReference expression = NULL;
    String name = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_CLASS &&
        Token_type(token) != TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("Not VariableExpression.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = Token_buffer(token);
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
        Logger_dbg("Not period. this is not ExpressionClass.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    Logger_dbg("ExpressionClass name = %s", name);
    ExpressionClass klass = NULL;
    klass = Memory_malloc(sizeof(struct ExpressionClassTag));
    memset(klass, 0x00, sizeof(struct ExpressionClassTag));
    klass->name = String_clone(name);
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_CLASS;
    expression->of.klass = klass;

	Logger_dbg("Next ReferenceExprssion parse.");
	Parser_next(parser);
    expression->next = ExpressionReference_parse(parser);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



//==============================
//ExpressionReference
//==============================
CriaId
ExpressionReference_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ExpressionReference expression,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    switch (expression->type)
    {
    case REFERENCE_EXPRESSION_TYPE_SELF:
    	Logger_dbg("expression->type = REFERENCE_EXPRESSION_TYPE_SELF, variable->name = %s", expression->of.variable->name);
    	Logger_dbg("object as parent is %p", object);
    	expression = expression->next;
        id = ExpressionReference_evaluate(interpreter, object, parameters, expression, object);
        break;
    case REFERENCE_EXPRESSION_TYPE_VARIABLE:
    	Logger_dbg("expression->type = REFERENCE_EXPRESSION_TYPE_VARIABLE, variable->name = %s", expression->of.variable->name);
    	Logger_dbg("parent is %p", parent);
        id = ExpressionVariable_evaluate(interpreter, object, parameters, expression->of.variable, parent);
        break;
    case REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL:
    	Logger_dbg("expression->type = REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL");
    	Logger_dbg("parent is %p", parent);
        id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, expression->of.function, parent);
        break;
    case REFERENCE_EXPRESSION_TYPE_CLASS:
    	Logger_dbg("expression->type = REFERENCE_EXPRESSION_TYPE_CLASS");
        id = ExpressionClass_evaluate(interpreter, object, parameters, expression->of.klass, parent);
        break;
    case REFERENCE_EXPRESSION_TYPE_GENERATE:
    	Logger_dbg("expression->type = REFERENCE_EXPRESSION_TYPE_GENERATE");
        id = ExpressionGenerate_evaluate(interpreter, object, parameters, expression->of.generate);
        break;
    default:
        break;
    }
    
    if (expression->next != NULL)
    {
		Logger_dbg("Evaluate next ReferenceExpression.");
		id = ExpressionReference_evaluate(interpreter, object, parameters, expression->next, id);
    }
    
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



ExpressionReference
ExpressionReference_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionReference expression = NULL;
    
    expression = ExpressionSelf_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of Self.");
        goto END;
    }
    
    expression = ExpressionClass_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of Self.");
        goto END;
    }
    
    expression = ExpressionFunctionCall_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of FunctionCall.");
        goto END;
    }
    
    expression = ExpressionVariable_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of Variable.");
        goto END;
    }
    
    expression = ExpressionGenerate_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of Generate.");
        goto END;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ExpressionParameters
ExpressionParameters_new(
    List parameterList
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionParameters parameters = NULL;
    parameters = Memory_malloc(sizeof(struct ExpressionParametersTag));
    memset(parameters, 0x00, sizeof(struct ExpressionParametersTag));
    parameters->list = parameterList;
    Logger_trc("[  END  ]%s", __func__);
    return parameters;
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
    
    List list = List_new();
    CriaId id = NULL;
    int count = List_count(expressions);
    int i = 0;
    
    for (i = 0; i < count; i++)
    {
        expression = (Expression)(List_get(expressions, i));
        switch (expression->kind)
        {
        case EXPRESSION_KIND_STRING_LITERAL:
            Logger_dbg("Do 'String literal expression'");
            id = (CriaId)ExpressionStringLiteral_evaluate(interpreter, object, parameterList, expression->of._stringLiteral_);
            Logger_dbg("Done 'String literal expression'");
            List_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_INTEGER_LITERAL:
            Logger_dbg("Do 'Integer literal expression'");
            id = ExpressionIntegerLiteral_evaluate(interpreter, object, parameterList, expression->of._integerLiteral_);
            Logger_dbg("Done 'Integer literal expression'");
            List_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_BOOLEAN_LITERAL:
            Logger_dbg("Do 'Boolean literal expression'");
            id = ExpressionBooleanLiteral_evaluate(interpreter, object, parameterList, expression->of._booleanLiteral_);
            Logger_dbg("Done 'Boolean literal expression'");
            List_add(list, id);
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
            id = ExpressionFunctionCall_evaluate(interpreter, object, parameterList, expression->of._functionCall_, NULL);
            Logger_dbg("Done 'Function call expression'");
            List_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_REFERENCE:
            Logger_dbg("Do reference expression");
            id = ExpressionReference_evaluate(interpreter, object, parameterList, expression->of._reference_, NULL);
            Logger_dbg("Done reference expression");
            List_add(list, id);
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
            List_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        default:
            break;
        }
    }
    
    
    Logger_trc("[  END  ]%s", __func__);
    return list;
}



ExpressionParameters
ExpressionParameters_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionParameters parameters = NULL;
    Expression expression = NULL;
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    List list = List_new();
    
    
    token = Parser_getCurrent(parser);
    Token_log(token);
    Logger_dbg("Loop start.");
    while (Token_type(token) != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Parse expression.");
        expression = Expression_parse(parser);
        if (expression == NULL)
        {
            Logger_dbg("expression is NULL.");
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
        
        
        Logger_dbg("Add expression.");
        List_add(list, expression);
        
        
        Logger_dbg("Get current token.");
        token = Parser_getCurrent(parser);
        Token_log(token);
        if (Token_type(token) == TOKEN_TYPE_COMMA)
        {
            Logger_dbg("Token is Comma.");
            if (Parser_next(parser) == FALSE)
            {
                Parser_setPosition(parser, position);
                Parser_error(token);
                goto END;
            }
            token = Parser_getCurrent(parser);
            continue;
        }
        
        
        Logger_dbg("Token is not right parenthesis.");
        if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            Token_log(token);
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
    }
    Logger_dbg("Loop end.");
    
    Logger_dbg("list count = %d", List_count(list));
    
    
    parameters = ExpressionParameters_new(list);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return parameters;
}



DefinitionFunction
ExpressionFunctionCall_searchFromObject(
	Interpreter interpreter,
    CriaId object,
    String functionName
)
{
    Logger_trc("[ START ]%s(function name is '%s')", __func__, functionName);
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;
    Hash i_methods = NULL;
    
	Logger_dbg("Object is %p.", object);
    if (object == NULL)
    {
		Logger_dbg("Object is NULL.");
    	goto END;
    }
    
    
	Logger_dbg("Search class named '%s'", object->name);
	klass = (DefinitionClass)Hash_get(Interpreter_classes(interpreter), object->name);
	if (klass == NULL)
	{
		Logger_dbg("Not found class named '%s'", object->name);
		goto END;
	}
	
	
	Logger_dbg("Search method named '%s'", functionName);
	
	i_methods = DefinitionClass_getMethods(klass, FALSE);
	Hash_log_key(i_methods);
	function = Hash_get(i_methods, functionName);
	if (function == NULL)
	{
		Logger_dbg("Not found method named '%s'.", functionName);
		goto END;
	}
	
	Logger_dbg("Method pointer is %p.", function);
	
END:
    Logger_trc("[  END  ]%s", __func__);
    return function;
}



DefinitionFunction
ExpressionFunctionCall_searchFromInterpreter(
	Interpreter interpreter,
    String functionName
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction function = NULL;


	Logger_dbg("Search function named '%s'", functionName);
	function = (DefinitionFunction)Hash_get(Interpreter_functions(interpreter), functionName);
	if (function == NULL)
	{
		Logger_dbg("Function is not found.");
		goto END;
	}
	
	
	Logger_dbg("Function pointer is %p.", function);

END:
    Logger_trc("[  END  ]%s", __func__);
    return function;
}



CriaId
ExpressionFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionFunctionCall expression,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaId current = NULL;
    DefinitionFunction  function = NULL;
    
    
    if (parent != NULL)
    {
    	current = parent;
		function = ExpressionFunctionCall_searchFromObject(interpreter, parent, expression->name);
		if (function != NULL)
			goto EVALUATE;
    }
	else
	{
		current = object;
		function = ExpressionFunctionCall_searchFromInterpreter(interpreter, expression->name);
		if (function != NULL)
			goto EVALUATE;
	}
	

	if (function == NULL)
	{
		Logger_err("Function '%s' is not found.", expression->name);
		runtime_error(interpreter);
		goto END;
	}
	    
    
EVALUATE:
    
    //引数の式を実行
    Logger_dbg("expression->name->pointer = %s", expression->name);
    Logger_dbg("expression->parameters->list->count = %d", List_count(expression->parameters->list));
    List parameters = ExpressionParameters_evaluate(interpreter, object, parameterList, expression->parameters);
    Logger_dbg("execute parameters count is '%d'", List_count(parameters));
    
    
    Logger_dbg("Call cria function.(%s)", expression->name);
    id = DefinitionFunction_evaluate(interpreter, current, DefinitionFunction_getParameterList(function), function, parameters, parent);
    
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



//==============================
//ExpressionGenerate
//==============================
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
    klass = (DefinitionClass)Hash_get(Interpreter_classes(interpreter), expression->name);
    if (klass == NULL)
    {
        Logger_dbg("Class is not found.");
        runtime_error(interpreter);
        goto END;
    }
    Logger_dbg("Class matched.");
    
    
    //引数の式を実行
    List parameters = ExpressionParameters_evaluate(interpreter, object, parameterList, expression->parameters);
    
    
	id = DefinitionClass_generateInstance(interpreter, klass, parameters);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



ExpressionReference
ExpressionGenerate_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    ExpressionReference expression = NULL;
    String name = NULL;
    ExpressionParameters parameters = NULL;
    
    Token token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_CLASS_LITERAL)
    {
    	Token_log(token);
    	Logger_dbg("Not class literal.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
    name = Token_buffer(token);
    Logger_dbg("Class name is '%s'", name);
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
    	Token_log(token);
        Logger_dbg("Second token is not left parenthesis.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
    Parser_next(parser);
    parameters = ExpressionParameters_parse(parser);
    if (parameters == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = Parser_getCurrent(parser);
    
    
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
    	Token_log(token);
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    ExpressionGenerate generate = Memory_malloc(sizeof(struct ExpressionGenerateTag));
    memset(generate, 0x00, sizeof(struct ExpressionGenerateTag));
    generate->name = String_clone(name);
    generate->parameters = parameters;
    Logger_dbg("Created GenerateExpression");
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_GENERATE;
    expression->of.generate = generate;
    
    
    Parser_next(parser);
    if (Token_type(token) == TOKEN_TYPE_PERIOD)
    {
        expression->next = ExpressionReference_parse(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
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
    char* buffer = expression->value;
    char* start = &buffer[1];
    char* next = NULL;
    StringBuffer stringBuffer = StringBuffer_new();
    
    
    Logger_dbg("Literal is '%s'", buffer);
    
    while (*start != '\0')
    {
        Logger_dbg("literal check");
        next = strchr(start, '\\');
        if (next == NULL)
        {
            Logger_dbg("set all ");
            StringBuffer_append(stringBuffer, start);
            break;
        }
        
        long length = next - start;
        char* tmp = Memory_malloc(length + 1);
        memset(tmp, 0x00, length + 1);
        strncpy(tmp, start, length);
        StringBuffer_append(stringBuffer, tmp);
        
        switch (*(next + 1))
        {
        case 'n':
            StringBuffer_appendChar(stringBuffer, '\n');
            break;
        case '"':
            StringBuffer_appendChar(stringBuffer, '"');
            break;
        default:
            StringBuffer_appendChar(stringBuffer, *(next + 1));
            break;
        }
        
        start = next + 2;
    }
    
    
    String value = StringBuffer_toString(stringBuffer);
    StringBuffer_dispose(stringBuffer);
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
        id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, expression->of._functionCall_, NULL);
        Logger_dbg("Done 'Function call expression'");
        break;
    case EXPRESSION_KIND_REFERENCE:
        Logger_dbg("Do reference expression");
        id = ExpressionReference_evaluate(interpreter, object, parameters, expression->of._reference_, NULL);
        Logger_dbg("Done reference expression");
        break;
    case EXPRESSION_KIND_VARIABLE:
        Logger_dbg("Do variable expression");
        id = ExpressionVariable_evaluate(interpreter, object, parameters, expression->of._variable_, NULL);
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



ExpressionReference
ExpressionVariable_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    ExpressionReference expression = NULL;
    String name = NULL;
    Boolean isStatic = FALSE;
    Boolean isConstant = FALSE;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER &&
        Token_type(token) != TOKEN_TYPE_CONSTANT)
    {
        Logger_dbg("Not VariableExpression.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = Token_buffer(token);
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Maybe FunctionCallExpression name = %s", name);
        Parser_setPosition(parser, position);
        goto END;
    }
    
    Logger_dbg("VariableExpression name = %s", name);
    ExpressionVariable variable = NULL;
    variable = Memory_malloc(sizeof(struct ExpressionVariableTag));
    memset(variable, 0x00, sizeof(struct ExpressionVariableTag));
    variable->name = String_clone(name);
    variable->isStatic = isStatic;
    variable->isConstant = isConstant;
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_VARIABLE;
    expression->of.variable = variable;

	Token_log(token);
    if (Token_type(token) == TOKEN_TYPE_PERIOD)
    {
    	Logger_dbg("Next ReferenceExprssion parse.");
    	Parser_next(parser);
        expression->next = ExpressionReference_parse(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ExpressionReference
ExpressionFunctionCall_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    ExpressionReference expression = NULL;
    String name = NULL;
    ExpressionParameters parameters = NULL;
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
    	Token_log(token);
		Logger_dbg("First token is not identifier.");
        Parser_setPosition(parser, position);
        goto END;
    }
	Logger_dbg("First token is identifier.");
    
    
    name = Token_buffer(token);
	Logger_dbg("name = '%s'", name);
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Second token is not left parenthesis.");
        Parser_setPosition(parser, position);
        goto END;
    }
	Logger_dbg("Second token is left parenthesis.");
    
    
    Parser_next(parser);
    parameters = ExpressionParameters_parse(parser);
    if (parameters == NULL)
    {
        Logger_dbg("The tokens that before right parenthesis are not parameters.");
        Parser_setPosition(parser, position);
        goto END;
    }
	Logger_dbg("The tokens that before right parenthesis are parameters.");
    
    
    token = Parser_getCurrent(parser);
    
    
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
	Logger_dbg("Last token is right parenthesis.");
    
    
    ExpressionFunctionCall function = Memory_malloc(sizeof(struct ExpressionFunctionCallTag));
    memset(function, 0x00, sizeof(struct ExpressionFunctionCallTag));
    function->name = String_clone(name);
    function->parameters = parameters;
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    memset(expression, 0x00, sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL;
    expression->of.function = function;
    
    
    Parser_next(parser);
    if (Token_type(token) == TOKEN_TYPE_PERIOD)
    {
        expression->next = ExpressionReference_parse(parser);
		Logger_dbg("Created next expression.");
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
ExpressionFactor_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    Token token = Parser_getCurrent(parser);
    Token_log(token);
    
    
    Logger_dbg("Check integer literal.");
    if (Token_type(token) == TOKEN_TYPE_INTEGER_LITERAL)
    {
        Logger_dbg("This is an integer literal token.");
        ExpressionIntegerLiteral integerLiteral = Memory_malloc(sizeof(struct ExpressionIntegerLiteralTag));
        memset(integerLiteral, 0x00, sizeof(struct ExpressionIntegerLiteralTag));
        integerLiteral->value = String_toInteger(Token_buffer(token));
        
        expression = Expression_new(EXPRESSION_KIND_INTEGER_LITERAL);
        expression->of._integerLiteral_ = integerLiteral;
        
        Parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check boolean literal.");
    if (Token_type(token) == TOKEN_TYPE_BOOLEAN_LITERAL)
    {
        Logger_dbg("This is an integer literal token.");
        ExpressionBooleanLiteral booleanLiteral = Memory_malloc(sizeof(struct ExpressionBooleanLiteralTag));
        memset(booleanLiteral, 0x00, sizeof(struct ExpressionBooleanLiteralTag));
        booleanLiteral->value = String_toBoolean(Token_buffer(token));
        
        expression = Expression_new(EXPRESSION_KIND_BOOLEAN_LITERAL);
        expression->of._booleanLiteral_ = booleanLiteral;
        
        Parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check string literal.");
    if (Token_type(token) == TOKEN_TYPE_STRING_LITERAL)
    {
        Logger_dbg("This is a string literal token.");
        ExpressionStringLiteral stringLiteral = Memory_malloc(sizeof(struct ExpressionStringLiteralTag));
        memset(stringLiteral, 0x00, sizeof(struct ExpressionStringLiteralTag));
        stringLiteral->value = String_clone(Token_buffer(token));
        
        expression = Expression_new(EXPRESSION_KIND_STRING_LITERAL);
        expression->of._stringLiteral_ = stringLiteral;
        
        Parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check new expression.");
    if (Token_type(token) == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("This is a left parenthesis token.");
        Parser_next(parser);
        expression = Expression_parse(parser);
        token = Parser_getCurrent(parser);
        if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            Parser_error(token);
        }
        Parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check reference expression.");
    if (Token_type(token) == TOKEN_TYPE_PERIOD ||
        Token_type(token) == TOKEN_TYPE_IDENTIFIER ||
        Token_type(token) == TOKEN_TYPE_CLASS ||
        Token_type(token) == TOKEN_TYPE_CLASS_LITERAL ||
        Token_type(token) == TOKEN_TYPE_CONSTANT)
    {
        ExpressionReference reference = ExpressionReference_parse(parser);
        if (reference == NULL)
        {
            Logger_err("Not reference expression.");
            goto END;
        }
        
        expression = Expression_new(EXPRESSION_KIND_REFERENCE);
        expression->of._reference_ = reference;
        goto END;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
ExpressionMultiplyDivide_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = ExpressionFactor_parse(parser);
    
    
    token = Parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) == TOKEN_TYPE_MULTIPLY)
    {
        kind = OPERATION_KIND_MULTIPLY;
    }
    else if (Token_type(token) == TOKEN_TYPE_DEVIDE)
    {
        kind = OPERATION_KIND_DIVIDE;
    }
    else
    {
        expression = left;
        goto END;
    }

    
    Parser_next(parser);
    right = Expression_parse(parser);
    
    
    Logger_dbg("Create MultiplyDivide Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = Expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
ExpressionPlusMinus_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = ExpressionMultiplyDivide_parse(parser);
    
    
    token = Parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) == TOKEN_TYPE_PLUS)
    {
        kind = OPERATION_KIND_PLUS;
    }
    else if (Token_type(token) == TOKEN_TYPE_MINUS)
    {
        kind = OPERATION_KIND_MINUS;
    }
    else
    {
        expression = left;
        goto END;
    }

    
    Parser_next(parser);
    right = Expression_parse(parser);
    
    
    Logger_dbg("Create PlusMinus Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = Expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
ExpressionCompare_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = ExpressionPlusMinus_parse(parser);
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_EQUAL)
    {
        kind = OPERATION_KIND_EQUAL;
    }
    else if (Token_type(token) == TOKEN_TYPE_NOT_EQUAL)
    {
        kind = OPERATION_KIND_NOT_EQUAL;
    }
    else if (Token_type(token) == TOKEN_TYPE_LESS_EQUAL)
    {
        kind = OPERATION_KIND_LESS_EQUAL;
    }
    else if (Token_type(token) == TOKEN_TYPE_LESS_THAN)
    {
        kind = OPERATION_KIND_LESS_THAN;
    }
    else
    {
        expression = left;
        goto END;
    }

    
    Parser_next(parser);
    right = Expression_parse(parser);
    
    
    Logger_dbg("Create Compare Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = Expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
ExpressionNot_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    left = ExpressionCompare_parse(parser);
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_NOT_EQUAL)
    {
        kind = OPERATION_KIND_NOT_EQUAL;
    }
    else
    {
        expression = left;
        goto END;
    }

    Parser_next(parser);
    right = Expression_parse(parser);
    
    Logger_dbg("Create NotEqual Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = Expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
ExpressionAndOr_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = NULL;
    Expression left = NULL;
    Expression right = NULL;
    ExpressionOperation operation = NULL;
    Token token = NULL;
    OperationKind kind;
    
    
    left = ExpressionCompare_parse(parser);
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_OR)
    {
        kind = OPERATION_KIND_OR;
    }
    else if (Token_type(token) == TOKEN_TYPE_AND)
    {
        kind = OPERATION_KIND_AND;
    }
    else
    {
        expression = left;
        goto END;
    }

    
    Parser_next(parser);
    right = Expression_parse(parser);
    
    
    Logger_dbg("Create AndOr Expression.");
    operation = Memory_malloc(sizeof(struct ExpressionOperationTag));
    memset(operation, 0x00, sizeof(struct ExpressionOperationTag));
    operation->kind = kind;
    operation->left = left;
    operation->right = right;
    
    expression = Expression_new(EXPRESSION_KIND_OPERATION);
    expression->of._operation_ = operation;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



Expression
Expression_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    
    expression = ExpressionAndOr_parse(parser);
    
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



