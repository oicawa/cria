/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "StringBuffer.h"
#include "CriaBoolean.h"
#include "CriaInteger.h"
#include "CriaString.h"
#include "CriaObject.h"
#include "CriaClass.h"
#include "CriaBlock.h"
#include "Definition.h"
#include "Hash.h"
#include "Statement.h"
#include "Runtime.h"

#include "Expression.h"


ExpressionReference ExpressionStringLiteral_parse(Parser parser);
ExpressionReference ExpressionIntegerLiteral_parse(Parser parser);
ExpressionReference ExpressionBooleanLiteral_parse(Parser parser);
ExpressionReference ExpressionVariable_parse(Parser parser);
ExpressionReference ExpressionGenerate_parse(Parser parser);
ExpressionReference ExpressionClass_parse(Parser parser);
ExpressionParameters ExpressionParameters_parse(Parser parser);
CriaId ExpressionBlock_evaluate(Interpreter interpreter, CriaId object, List parameterList, ExpressionBlock expression);
ExpressionBlock ExpressionBlock_parse(Parser parser, Boolean start_with_reserved);




Expression
Expression_new(
    ExpressionKind  kind
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression expression = Memory_malloc(sizeof(struct ExpressionTag));
    expression->kind = kind;
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



//==============================
//ExpressionOperation
//==============================
CriaId
ExpressionOperation_evaluate_of_null(
    Interpreter interpreter,
    OperationKind operation,
    CriaId left,
    CriaId right
)
{
    CriaId id = NULL;
    
    if (left == NULL && right != NULL)
        id = right;
    else if (left != NULL && right == NULL)
        id = left;
    else if (left == NULL && right == NULL)
        id = NULL;
    else
        Runtime_error(interpreter, "Both left & right expression are not 'null'.");
    
    
    switch (operation)
    {
    case OPERATION_KIND_EQUAL:
        if (id == NULL)
            id = (CriaId)CriaBoolean_new(FALSE, TRUE);
        else
            id = (CriaId)CriaBoolean_new(FALSE, FALSE);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (id == NULL)
            id = (CriaId)CriaBoolean_new(FALSE, FALSE);
        else
            id = (CriaId)CriaBoolean_new(FALSE, TRUE);
        break;
    case OPERATION_KIND_PLUS:
        break;
    case OPERATION_KIND_MINUS:
    case OPERATION_KIND_MULTIPLY:
    case OPERATION_KIND_DIVIDE:
    case OPERATION_KIND_MODULO:
    case OPERATION_KIND_LESS_THAN:
    case OPERATION_KIND_LESS_EQUAL:
    case OPERATION_KIND_OR:
    case OPERATION_KIND_AND:
    default:
        Runtime_error(interpreter, "Illegal operation kind.");
        break;
    }
    
    return id;
}



CriaId
ExpressionOperation_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    CriaBlock block,
    ExpressionOperation expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    if (expression->left == NULL)
    {
        Logger_dbg("expression->left is NULL!");
    }
    CriaId left = Expression_evaluate(interpreter, object, parameters, block, expression->left);
    Logger_dbg("Evaluated left expression.");
    CriaId right = Expression_evaluate(interpreter, object, parameters, block, expression->right);
    Logger_dbg("Evaluated right expression.");
    
    
    if (left == NULL || right == NULL)
    {
        id = ExpressionOperation_evaluate_of_null(interpreter, expression->kind, left, right);
        goto END;
    }
    
    Logger_dbg("Check data type.");
    Logger_dbg("Data type = %d. (INTEGER is %d)", left->type, CRIA_DATA_TYPE_INTEGER);
    Logger_dbg("Checked data type.");
    
    //式の左辺右辺の型チェック
    if (left->type != right->type)
    {
        Runtime_error(interpreter, "left & right expression type is not same.");
    }
    
    //演算可能・不可能のチェック
    if (left->type == CRIA_DATA_TYPE_FUNCTION
        || left->type == CRIA_DATA_TYPE_CRIA_OBJECT
        || left->type == CRIA_DATA_TYPE_CRIA_FUNCTION)
    {
        Runtime_error(interpreter, "Not operatable.");
    }
    
    switch (left->type)
    {
    case CRIA_DATA_TYPE_BOOLEAN:
        id = CriaBoolean_operate(interpreter, expression->kind, (CriaBoolean)left, (CriaBoolean)right);
        break;
    case CRIA_DATA_TYPE_INTEGER:
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
        Runtime_error(interpreter, "Not supported operation.");
        break;
    }
    
END:
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
    if (token->type != TOKEN_TYPE_PERIOD)
    {
        Logger_dbg("Not self object.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    Logger_dbg("VariableExpression name = '_self_'");
    ExpressionVariable variable = NULL;
    variable = Memory_malloc(sizeof(struct ExpressionVariableTag));
    variable->name = String_new(L"_self_");
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
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
    CriaBlock block,
    ExpressionVariable  expression,
    CriaId parent
)
{
    Logger_trc("[ START ]%s (variable name = '%s')", __func__, expression->name);
    CriaId id = NULL;
    
    
    Logger_dbg("parent is %p", parent);
    if (parent == NULL)
    {
        if (wcscmp(expression->name, L"<<block>>") == 0)
        {
            id = (CriaId)block;
		    if (id != NULL)
			    goto END;
        }
        else
        {
		    id = ExpressionVariable_evaluateFromParameters(interpreter, parameterList, expression->name);
		    if (id != NULL)
			    goto END;
		
		    id = ExpressionVariable_evaluateFromInterpreter(interpreter, expression->name);
		    if (id != NULL)
			    goto END;
        }
        
		
        Runtime_error(interpreter, "The variable '%s' is not found in parameter variables or global variables.", expression->name);
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
    CriaBlock block,
    ExpressionClass klass,
    CriaId parent
)
{
    Logger_trc("[ START ]%s (class name = '%s')", __func__, klass->name);
    CriaId id = NULL;
    String className = NULL;
    DefinitionClass definition = NULL;
    Hash classes = NULL;

    
    classes = Interpreter_classes(interpreter);
    if (wcscmp(klass->name, L"class") != 0)
    {
        definition = (DefinitionClass)Hash_get(classes, klass->name);
    }
    else if (object->type == CRIA_DATA_TYPE_CRIA_OBJECT)
    {
        definition = (DefinitionClass)Hash_get(classes, object->name);
    }
    else if (object->type == CRIA_DATA_TYPE_CRIA_CLASS)
    {
        definition = ((CriaClass)object)->definition;
    }
    className = DefinitionClass_getName(definition);
    Logger_dbg("Real class name = '%s'.", className);
    
    
    if (definition == NULL)
    {
        Logger_err("Specified class not found. ('%s')", className);
        Runtime_error(interpreter, "Specified class '%s' is not found.", className);
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
    if (token->type != TOKEN_TYPE_CLASS &&
        token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("Not ExpressionClass.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->value;
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD)
    {
        Logger_dbg("Not period. this is not ExpressionClass.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    Logger_dbg("ExpressionClass name = %s", name);
    ExpressionClass klass = NULL;
    klass = Memory_malloc(sizeof(struct ExpressionClassTag));
    klass->name = String_new(name);
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
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
    CriaBlock block,
    ExpressionReference expression,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    switch (expression->type)
    {
    case REFERENCE_EXPRESSION_TYPE_SELF:
        id = object;
        break;
    case REFERENCE_EXPRESSION_TYPE_VARIABLE:
        id = ExpressionVariable_evaluate(interpreter, object, parameters, block, expression->of.variable, parent);
        break;
    case REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL:
        id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, block, expression->of.function, parent);
        break;
    case REFERENCE_EXPRESSION_TYPE_INDEXER:
        id = ExpressionIndexer_evaluate(interpreter, object, parameters, expression->of.indexer, parent);
        break;
    case REFERENCE_EXPRESSION_TYPE_CLASS:
        id = ExpressionClass_evaluate(interpreter, object, parameters, block, expression->of.klass, parent);
        break;
    case REFERENCE_EXPRESSION_TYPE_GENERATE:
        id = ExpressionGenerate_evaluate(interpreter, object, parameters, block, expression->of.generate);
        break;
    case REFERENCE_EXPRESSION_TYPE_STRING:
        id = ExpressionStringLiteral_evaluate(interpreter, object, parameters, expression->of.string);
        break;
    case REFERENCE_EXPRESSION_TYPE_INTEGER:
        id = ExpressionIntegerLiteral_evaluate(interpreter, object, parameters, expression->of.integer);
        break;
    case REFERENCE_EXPRESSION_TYPE_BOOLEAN:
        id = ExpressionBooleanLiteral_evaluate(interpreter, object, parameters, expression->of.boolean);
        break;
    default:
        break;
    }
    
    if (expression->next != NULL)
    {
		Logger_dbg("Evaluate next ReferenceExpression.");
		id = ExpressionReference_evaluate(interpreter, object, parameters, block, expression->next, id);
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
    
    expression = ExpressionIndexer_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of Indexer.");
        goto END;
    }
    
    expression = ExpressionStringLiteral_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of string literal.");
        goto END;
    }
    
    expression = ExpressionIntegerLiteral_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of integer literal.");
        goto END;
    }
    
    expression = ExpressionBooleanLiteral_parse(parser);
    if (expression != NULL)
    {
    	Logger_dbg("Created Expression of boolean literal.");
        goto END;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



List
ExpressionParameters_get_list(
	ExpressionParameters parameters
)
{
    return parameters->list;
}



ExpressionParameters
ExpressionParameters_new(
    List parameterList
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionParameters parameters = NULL;
    parameters = Memory_malloc(sizeof(struct ExpressionParametersTag));
    parameters->list = parameterList;
    Logger_trc("[  END  ]%s", __func__);
    return parameters;
}



List
ExpressionParameters_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    CriaBlock block,
    ExpressionParameters parametersExpression
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
        case EXPRESSION_KIND_FUNCTION_CALL:
            Logger_dbg("Do 'Function call expression'");
            id = ExpressionFunctionCall_evaluate(interpreter, object, parameterList, block, expression->of._functionCall_, NULL);
            Logger_dbg("Done 'Function call expression'");
            List_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_BLOCK:
            id = ExpressionBlock_evaluate(interpreter, object, parameterList, expression->of._block_);
            List_add(list, id);
            break;
        case EXPRESSION_KIND_REFERENCE:
            Logger_dbg("Do reference expression");
            id = ExpressionReference_evaluate(interpreter, object, parameterList, block, expression->of._reference_, NULL);
            Logger_dbg("Done reference expression");
            List_add(list, id);
            Logger_dbg("Add 'Cria Id'");
            break;
        case EXPRESSION_KIND_OPERATION:
            Logger_dbg("Do operation expression");
            id = ExpressionOperation_evaluate(interpreter, object, parameterList, block, expression->of._operation_);
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



CriaId
ExpressionBlock_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionBlock expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBlock id = NULL;
    DefinitionFunction  function = NULL;
    
    
	if (expression == NULL)
	{
		Runtime_error(interpreter, "Block expression is NULL.");
		goto END;
	}
    
    
    function = expression->function;
	if (function == NULL)
	{
		Logger_err("Function in the block is NULL.");
		Runtime_error(interpreter, "Function in the block is NULL.");
		goto END;
	}
    
    //この時点では、ブロックにどんな引数が渡されるかまだ不明。
    //なので、ここで記述すべきは、「ブロックが定義された場所の環境を保存するロジック」
    
    id = CriaBlock_new(interpreter, object, parameterList, function);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)id;
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
    Logger_dbg("Loop start.");
    while (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT &&
           token->type != TOKEN_TYPE_BRACKET_RIGHT)
    {
        Logger_dbg("Parse expression.");
        expression = Expression_parse(parser);
        if (expression == NULL)
        {
            Logger_dbg("expression is NULL.");
            Parser_setPosition(parser, position);
            Parser_error(parser, token);
            goto END;
        }
        
        
        Logger_dbg("Add expression.");
        List_add(list, expression);
        
        
        Logger_dbg("Get current token.");
        token = Parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_COMMA)
        {
            Logger_dbg("Token is Comma.");
            if (Parser_next(parser) == FALSE)
            {
                Parser_setPosition(parser, position);
                Parser_error(parser, token);
                goto END;
            }
            token = Parser_getCurrent(parser);
            continue;
        }
        
        
        Logger_dbg("Token is not right parenthesis.");
        if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT &&
            token->type != TOKEN_TYPE_BRACKET_RIGHT)
        {
            Parser_setPosition(parser, position);
            Parser_error(parser, token);
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
ExpressionFunctionCall_searchFromClass(
	Interpreter interpreter,
    CriaId object,
    String functionName
)
{
    Logger_trc("[ START ]%s(function name is '%s')", __func__, functionName);
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;
    Hash s_methods = NULL;
    
	Logger_dbg("Object is %p.", object);
    if (object == NULL)
    {
		Logger_dbg("Object is NULL.");
    	goto END;
    }

    
    klass = ((CriaClass)object)->definition;
	
	
	Logger_dbg("Search method named '%s'", functionName);
	
	s_methods = DefinitionClass_getMethods(klass, TRUE);
	Hash_log_key(s_methods);
	function = Hash_get(s_methods, functionName);
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
    DefinitionVariable variable = NULL;
    CriaId id = NULL;


	Logger_dbg("Search function named '%s'", functionName);
	function = (DefinitionFunction)Hash_get(Interpreter_functions(interpreter), functionName);
	if (function != NULL)
		goto END;
    
    
    variable = (DefinitionVariable)Hash_get(Interpreter_variables(interpreter), functionName);
    if (variable == NULL)
        goto END;
    
	id = DefinitionVariable_getObject(variable);
    if (id == NULL)
        goto END;
    
    if (id->type != CRIA_DATA_TYPE_BLOCK)
        goto END;
    
    function = ((CriaBlock)id)->function;

END:
    Logger_trc("[  END  ]%s", __func__);
    return function;
}



ExpressionFunctionCall
ExpressionFunctionCall_new(
    String name,
    ExpressionParameters parameters
)
{
    ExpressionFunctionCall function = NULL;
    function = Memory_malloc(sizeof(struct ExpressionFunctionCallTag));
    function->name = String_new(name);
    function->parameters = parameters;
    function->block = NULL;
    
    return function;
}


DefinitionFunction
ExpressionFunctionCall_searchFromParameters(
    Interpreter interpreter,
    String name,
    List parameterList
)
{
    CriaId id = NULL;
    CriaBlock block = NULL;
    DefinitionFunction function = NULL;
    
    id = ExpressionVariable_evaluateFromParameters(interpreter, parameterList, name);
    
    if (id == NULL)
        goto END;
    
    if (id->type != CRIA_DATA_TYPE_BLOCK)
        goto END;
    
    block = (CriaBlock)id;
    
    function = block->function;
    
END:
    
    return function;
}



CriaId
ExpressionFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    CriaBlock block,
    ExpressionFunctionCall expression,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaId current = NULL;
    DefinitionFunction  function = NULL;
    List tmp = NULL;
    String target = NULL;
    List parameters = NULL;
    CriaBlock next_block = NULL;
    
    if (parent == NULL)
    {
        if (wcscmp(expression->name, L"<<block>>") == 0)
        {
            //TODO: block expression have to be a argument in this function.
            function = block->function;
            target = L"<<block>>";
        }
        else
        {
		    current = object;
		    function = ExpressionFunctionCall_searchFromInterpreter(interpreter, expression->name);
            target = L"Interpreter";
            
            //Search from parameters.
            if (function == NULL)
            {
                function = ExpressionFunctionCall_searchFromParameters(interpreter, expression->name, parameterList);
                target = L"Parameters";
            }
        }
    }
    else if (parent->type == CRIA_DATA_TYPE_CRIA_OBJECT ||
              parent->type == CRIA_DATA_TYPE_STRING ||
              parent->type == CRIA_DATA_TYPE_INTEGER)
    {
    	current = parent;
		function = ExpressionFunctionCall_searchFromObject(interpreter, parent, expression->name);
        target = parent->name;
    }
    else if (parent->type == CRIA_DATA_TYPE_CRIA_CLASS)
    {
    	current = parent;
		function = ExpressionFunctionCall_searchFromClass(interpreter, parent, expression->name);
        target = parent->name;
    }
    

	if (function == NULL)
	{
		Logger_err("Function '%s' is not found.", expression->name);
		Runtime_error(interpreter, "Function '%s' is not found in '%s'.", expression->name, target);
		goto END;
	}
	

    if (expression->block != NULL)
    {
        next_block = (CriaBlock)ExpressionBlock_evaluate(interpreter, object, parameterList, expression->block);
    }
    else
    {
        next_block = block;
    }
    
    parameters = ExpressionParameters_evaluate(interpreter, object, parameterList, next_block, expression->parameters);
    tmp = DefinitionFunction_getParameterList(function);
    id = DefinitionFunction_evaluate(interpreter, current, tmp, next_block, function, parameters, parent);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
ExpressionIndexer_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionIndexer expression,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaId current = NULL;
    DefinitionFunction  function = NULL;
    List tmp = NULL;
    
    
    if (parent == NULL)
    {
        Runtime_error(interpreter, "Parent object of indexer is NULL.");
        goto END;
    }
    else if (parent->type == CRIA_DATA_TYPE_CRIA_OBJECT ||
              parent->type == CRIA_DATA_TYPE_STRING)
    {
    	current = parent;
		function = ExpressionFunctionCall_searchFromObject(interpreter, parent, L"get[]");
    }
    else
    {
        Runtime_error(interpreter, "Data type of arent object is not CriaObject or String.");
        goto END;
    }
    

	if (function == NULL)
	{
		Logger_err("Function '%s' is not found.", "get[]");
		Runtime_error(interpreter, "Function 'get[]' is not found.");
		goto END;
	}
	
    
    //引数の式を実行
    List parameters = ExpressionParameters_evaluate(interpreter, object, parameterList, NULL, expression->parameters);
    
    tmp = DefinitionFunction_getParameterList(function);
    id = DefinitionFunction_evaluate(interpreter, current, tmp, NULL, function, parameters, parent);
    
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



ExpressionReference
ExpressionBooleanLiteral_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    ExpressionReference expression = NULL;
    
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_BOOLEAN_LITERAL_FALSE &&
        token->type != TOKEN_TYPE_BOOLEAN_LITERAL_TRUE)
    {
        Parser_setPosition(parser, position);
        goto END;
    }   
    
    
    ExpressionBooleanLiteral boolean = NULL;
    boolean = Memory_malloc(sizeof(struct ExpressionBooleanLiteralTag));
    boolean->value = String_toBoolean(token->value);
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_BOOLEAN;
    expression->of.boolean = boolean;

	
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Parser_next(parser);
        expression->next = ExpressionReference_parse(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



//==============================
//ExpressionGenerate
//==============================
CriaId
ExpressionGenerate_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameterList,
    CriaBlock block,
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
        Runtime_error(interpreter, "Class '%s' is not found.", String_wcsrtombs(expression->name));
        goto END;
    }
    Logger_dbg("Class matched.");
    
    
    //引数の式を実行
    List parameters = ExpressionParameters_evaluate(interpreter, object, parameterList, block, expression->parameters);
    
    
	id = DefinitionClass_generateInstance(interpreter, klass, parameters, block);
    
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
    if (token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
    	Logger_dbg("Not class literal.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
    name = token->value;
    Logger_dbg("Class name is '%s'", name);
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
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
    
    
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
    
    
    ExpressionGenerate generate = Memory_malloc(sizeof(struct ExpressionGenerateTag));
    generate->name = String_new(name);
    generate->parameters = parameters;
    Logger_dbg("Created GenerateExpression");
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_GENERATE;
    expression->of.generate = generate;
    
    
    Parser_next(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
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



ExpressionReference
ExpressionIntegerLiteral_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    ExpressionReference expression = NULL;
    
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_INTEGER_LITERAL)
    {
        Parser_setPosition(parser, position);
        goto END;
    }   
    
    
    ExpressionIntegerLiteral integer = NULL;
    integer = Memory_malloc(sizeof(struct ExpressionIntegerLiteralTag));
    integer->value = String_toInteger(token->value);
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_INTEGER;
    expression->of.integer = integer;

	
	Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Parser_next(parser);
        expression->next = ExpressionReference_parse(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
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
    char* tmp_buffer = NULL;
    char* tmp_start = NULL;
    char* tmp_next = NULL;
    char* tmp_tmp = NULL;
    
    CriaString string = NULL;
    String buffer = expression->value;
    String start = &buffer[1];
    String next = NULL;
    StringBuffer stringBuffer = StringBuffer_new();
    
    
    tmp_buffer = String_wcsrtombs(buffer);
    tmp_start = String_wcsrtombs(start);
    
    while (*start != L'\0')
    {
        Logger_dbg("literal check");
        next = wcschr(start, L'\\');
        if (next == NULL)
        {
            Logger_dbg("set all ");
            StringBuffer_append(stringBuffer, start);
            break;
        }
        tmp_next = String_wcsrtombs(next);
        
        long length = wcslen(start) - wcslen(next);
        String tmp = Memory_malloc(sizeof(wchar_t) * (length + 1));
        wcsncpy(tmp, start, length);
        tmp_tmp = String_wcsrtombs(tmp);
        StringBuffer_append(stringBuffer, tmp);
        
        switch (*(next + 1))
        {
        case L'n':
            StringBuffer_appendChar(stringBuffer, L'\n');
            break;
        case L'"':
            StringBuffer_appendChar(stringBuffer, L'"');
            break;
        default:
            StringBuffer_appendChar(stringBuffer, *(next + 1));
            break;
        }
        
        start = next + 2;
        tmp_start = String_wcsrtombs(start);
    }
    
    
    String value = StringBuffer_toString(stringBuffer);
    value[wcslen(value) - 1] = L'\0';
    Logger_dbg("Edited string is '%s'", value);
    
    
    string = CriaString_new(TRUE, value);
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)string;
}


ExpressionReference
ExpressionStringLiteral_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    ExpressionReference expression = NULL;
    
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_STRING_LITERAL)
    {
        Parser_setPosition(parser, position);
        goto END;
    }   
    
    
    ExpressionStringLiteral string = NULL;
    string = Memory_malloc(sizeof(struct ExpressionStringLiteralTag));
    string->value = String_new(token->value);
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_STRING;
    expression->of.string = string;

	
	Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Parser_next(parser);
        expression->next = ExpressionReference_parse(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



CriaId
Expression_evaluate(
    Interpreter interpreter,
    CriaId object,
	List parameters,
    CriaBlock block,
    Expression  expression
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    switch (expression->kind)
    {
    case EXPRESSION_KIND_GENERATE:
        Logger_dbg("Do 'Generate expression'");
        id = ExpressionGenerate_evaluate(interpreter, object, parameters, block, expression->of._generate_);
        Logger_dbg("Done 'Generate  expression'");
        break;
    case EXPRESSION_KIND_NULL:
        Logger_dbg("Do 'Null expression'");
        id = NULL;
        Logger_dbg("Done 'Generate  expression'");
        break;
    case EXPRESSION_KIND_FUNCTION_CALL:
        Logger_dbg("Do 'Function call expression'");
        id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, block, expression->of._functionCall_, NULL);
        Logger_dbg("Done 'Function call expression'");
        break;
    case EXPRESSION_KIND_REFERENCE:
        Logger_dbg("Do reference expression");
        id = ExpressionReference_evaluate(interpreter, object, parameters, block, expression->of._reference_, NULL);
        Logger_dbg("Done reference expression");
        break;
    case EXPRESSION_KIND_VARIABLE:
        Logger_dbg("Do variable expression");
        id = ExpressionVariable_evaluate(interpreter, object, parameters, block, expression->of._variable_, NULL);
        Logger_dbg("Done variable expression");
        break;
    case EXPRESSION_KIND_OPERATION:
        Logger_dbg("Do operation expression");
        id = ExpressionOperation_evaluate(interpreter, object, parameters, block, expression->of._operation_);
        Logger_dbg("Done operation expression");
        break;
    case EXPRESSION_KIND_BLOCK:
        //TODO: Herre, I must implement binding environmental variables to block.
        id = ExpressionBlock_evaluate(interpreter, object, parameters, expression->of._block_);
        break;
    default:
        Runtime_error(interpreter, "Illegal expression.");
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
    if (token->type == TOKEN_TYPE_IDENTIFIER ||
        token->type == TOKEN_TYPE_CONSTANT)
    {
        name = token->value;
    }
    else if (token->type == TOKEN_TYPE_BLOCK)
    {
        name = L"<<block>>";
    }
    else
    {
        Logger_dbg("Not VariableExpression.");
        Parser_setPosition(parser, position);
        goto END;
    }   
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("Maybe FunctionCallExpression name = %s", name);
        Parser_setPosition(parser, position);
        goto END;
    }
    
   
    Logger_dbg("VariableExpression name = %s", name);
    ExpressionVariable variable = NULL;
    variable = Memory_malloc(sizeof(struct ExpressionVariableTag));
    variable->name = String_new(name);
    variable->isStatic = isStatic;
    variable->isConstant = isConstant;
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_VARIABLE;
    expression->of.variable = variable;

	
    if (token->type == TOKEN_TYPE_PERIOD)
    {
    	Logger_dbg("Next ReferenceExprssion parse.");
    	Parser_next(parser);
        expression->next = ExpressionReference_parse(parser);
    }
    else if (token->type == TOKEN_TYPE_BRACKET_LEFT)
    {
    	Logger_dbg("Next ReferenceExprssion parse.");
        expression->next = ExpressionReference_parse(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ExpressionReference
ExpressionIndexer_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    ExpressionReference expression = NULL;
    ExpressionParameters parameters = NULL;
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_BRACKET_LEFT)
    {
		Logger_dbg("First token is not '['.");
        Parser_setPosition(parser, position);
        goto END;
    }
	Logger_dbg("First token is '['.");
    
    
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
    if (token->type != TOKEN_TYPE_BRACKET_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
	Logger_dbg("Last token is right parenthesis.");
    
    
    ExpressionIndexer indexer = Memory_malloc(sizeof(struct ExpressionIndexerTag));
    indexer->parameters = parameters;
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_INDEXER;
    expression->of.indexer = indexer;
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        expression->next = ExpressionReference_parse(parser);
		Logger_dbg("Created next expression.");
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
    Boolean is_block = FALSE;
    ExpressionBlock block = NULL;
    
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_IDENTIFIER)
    {
        name = token->value;
        is_block = FALSE;
    }
    else if (token->type == TOKEN_TYPE_BLOCK)
    {
        name = L"<<block>>";
        is_block = TRUE;
    }
    else
    {
		Logger_dbg("First token is not identifier.");
        Parser_setPosition(parser, position);
        goto END;
    }
        
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    
    if (token->type != TOKEN_TYPE_PARENTHESIS_LEFT)
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
    
    
    if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
        Logger_dbg("Last token is not right parenthesis.");
        goto END;
    }
	Logger_dbg("Last token is right parenthesis.");
    
    
    
    Parser_next(parser);
    block = ExpressionBlock_parse(parser, FALSE);
    
    
    ExpressionFunctionCall function = Memory_malloc(sizeof(struct ExpressionFunctionCallTag));
    function->name = String_new(name);
    function->parameters = parameters;
    function->block = block;
    
    expression = Memory_malloc(sizeof(struct ExpressionReferenceTag));
    expression->type = REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL;
    expression->of.function = function;
    
    
    if (block != NULL)
        goto END;
    
    
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        expression->next = ExpressionReference_parse(parser);
		Logger_dbg("Created next expression.");
    }

END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}



ExpressionBlock
ExpressionBlock_parse(
    Parser parser,
    Boolean start_with_reserved
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionBlock block = NULL;
    DefinitionFunction functionDefinition = NULL;
    Item restore = Parser_getPosition(parser);
    Boolean isStatic = FALSE;
    List parameters = NULL;
    List statements = NULL;
    Statement statement = NULL;
    Token token = NULL;

    
    if (start_with_reserved == TRUE)
    {
        token = Parser_getCurrent(parser);
        if (token->type != TOKEN_TYPE_BLOCK)
        {
            Logger_dbg("Not block.");
        	goto END;
        }
        Parser_next(parser);
        Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, TRUE);
    }
    else
    {
        if (!Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, FALSE))
            goto END;
    }
    
    
    
	parameters = DefinitionFunction_parse_parameters(parser);
	if (parameters == NULL)
    {
        Logger_dbg("Not parameters.");
    	goto END;
    }
	
    Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_RIGHT, TRUE);
    Parser_eat(parser, TOKEN_TYPE_COLON, TRUE);
    Parser_eat(parser, TOKEN_TYPE_NEW_LINE, TRUE);
    Parser_eat(parser, TOKEN_TYPE_INDENT, TRUE);
	
	statements = List_new();
    while(1)
    {
        token = Parser_getCurrent(parser);
        if (token->type == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            Parser_error(parser, token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        List_add(statements, statement);
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_DEDENT, TRUE) == FALSE)
    {
        Logger_dbg("Not <<DEDENT>>.");
    	goto END;
    }
	
    functionDefinition = DefinitionFunction_new(NULL, FALSE, isStatic, parameters, statements, NULL);
    
    block = Memory_malloc(sizeof(struct ExpressionBlockTag));
    block->function = functionDefinition;
    
END:
	if (block == NULL)
		Parser_setPosition(parser, restore);
	else
        Parser_insert(parser, TOKEN_TYPE_NEW_LINE);
    Logger_trc("[  END  ]%s", __func__);
    return block;
}



Expression
ExpressionFactor_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Expression  expression = NULL;
    Token token = Parser_getCurrent(parser);
    
    
    Logger_dbg("Check null literal.");
    if (token->type == TOKEN_TYPE_NULL)
    {
        Logger_dbg("This is a null literal token.");
        expression = Expression_new(EXPRESSION_KIND_NULL);
        Parser_next(parser);
        goto END;
    }
    
    Logger_dbg("Check new expression.");
    if (token->type == TOKEN_TYPE_PARENTHESIS_LEFT)
    {
        Logger_dbg("This is a left parenthesis token.");
        Parser_next(parser);
        expression = Expression_parse(parser);
        token = Parser_getCurrent(parser);
        if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
        {
            Parser_error(parser, token);
        }
        Parser_next(parser);
        goto END;
    }
    
    
    Logger_dbg("Check block expression.");
    if (token->type == TOKEN_TYPE_BLOCK)
    {
        ExpressionReference variable = ExpressionVariable_parse(parser);
        if (variable != NULL)
        {
            expression = Expression_new(EXPRESSION_KIND_REFERENCE);
            expression->of._reference_ = variable;
            goto END;
        }
        
        ExpressionBlock block = ExpressionBlock_parse(parser, TRUE);
        if (block != NULL)
        {
            expression = Expression_new(EXPRESSION_KIND_BLOCK);
            expression->of._block_ = block;
            goto END;
        }
    }
    
    Logger_dbg("Check reference expression.");
    if (token->type == TOKEN_TYPE_STRING_LITERAL ||
        token->type == TOKEN_TYPE_INTEGER_LITERAL ||
        token->type == TOKEN_TYPE_BOOLEAN_LITERAL_TRUE ||
        token->type == TOKEN_TYPE_BOOLEAN_LITERAL_FALSE ||
        token->type == TOKEN_TYPE_PERIOD ||
        token->type == TOKEN_TYPE_IDENTIFIER ||
        token->type == TOKEN_TYPE_CLASS ||
        token->type == TOKEN_TYPE_CLASS_LITERAL ||
        token->type == TOKEN_TYPE_CONSTANT)
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
    if (token->type == TOKEN_TYPE_MULTIPLY)
    {
        kind = OPERATION_KIND_MULTIPLY;
    }
    else if (token->type == TOKEN_TYPE_DEVIDE)
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
    if (token->type == TOKEN_TYPE_PLUS)
    {
        kind = OPERATION_KIND_PLUS;
    }
    else if (token->type == TOKEN_TYPE_MINUS)
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
    if (token->type == TOKEN_TYPE_EQUAL)
    {
        kind = OPERATION_KIND_EQUAL;
    }
    else if (token->type == TOKEN_TYPE_NOT_EQUAL)
    {
        kind = OPERATION_KIND_NOT_EQUAL;
    }
    else if (token->type == TOKEN_TYPE_LESS_EQUAL)
    {
        kind = OPERATION_KIND_LESS_EQUAL;
    }
    else if (token->type == TOKEN_TYPE_LESS_THAN)
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
    if (token->type == TOKEN_TYPE_NOT_EQUAL)
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
    if (token->type == TOKEN_TYPE_OR)
    {
        kind = OPERATION_KIND_OR;
    }
    else if (token->type == TOKEN_TYPE_AND)
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


void*
ExpressionReference_getReference(
    ExpressionReference reference
)
{
    Logger_trc("[ START ]%s", __func__);
    void* expression = NULL;
    
    if (reference == NULL)
        goto END;
    
    switch (reference->type)
    {
    case REFERENCE_EXPRESSION_TYPE_SELF:
    case REFERENCE_EXPRESSION_TYPE_VARIABLE:
        expression = reference->of.variable;
        break;
    case REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL:
        expression = reference->of.function;
        break;
    case REFERENCE_EXPRESSION_TYPE_CLASS:
        expression = reference->of.klass;
        break;
    case REFERENCE_EXPRESSION_TYPE_GENERATE:
        expression = reference->of.generate;
        break;
    default:
        goto END;
    }
        
END:
    Logger_trc("[  END  ]%s", __func__);
    return expression;
}

