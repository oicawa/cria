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
#include "String.h"
#include "List.h"
#include "Hash.h"
#include "Interpreter.h"
#include "CriaId.h"
#include "Statement.h"
#include "Runtime.h"
#include "Parser.h"
#include "CriaObject.h"
#include "Expression.h"
#include "Definition.h"



//==============================
//DefinitionVariable
//==============================
DefinitionVariableNormal
DefinitionVariableNormal_new(
    String name,
    Boolean isStatic,
    Boolean isConstant
)
{
    DefinitionVariableNormal normal = NULL;
    normal = Memory_malloc(sizeof(struct DefinitionVariableNormalTag));
    normal->name = String_new(name);
    normal->isStatic = isStatic;
    normal->isConstant = isConstant;

    return normal;
}



DefinitionVariable
DefinitionVariable_new(
    DefinitionVariableType type,
    String name,
    Boolean isStatic,
    Boolean isConstant,
    Item item
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = Memory_malloc(sizeof(struct DefinitionVariableTag));
    Logger_dbg("Created empty definition.");
    definition->type = type;
    Logger_dbg("Set definition type.");
    if (type == DEFINITION_VARIABLE_TYPE_NORMAL)
    {
        Logger_dbg("Normal variable.");
        definition->of.normal = DefinitionVariableNormal_new(name, isStatic, isConstant);
    }
    else
    {
        Logger_dbg("Item variable.");
        definition->of.item = item;
    }

    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



DefinitionVariable
DefinitionVariable_search(
    List    variables,
    String  name
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    DefinitionVariable tmp = NULL;
    int index = 0;
    
    Logger_dbg("variables pointer = %p", variables);
    if (variables == NULL)
    {
        Logger_dbg("Variables is NULL.");
        goto END;
    }
    
    if (name == NULL)
    {
        Logger_dbg("'name' is NULL.");
        goto END;
    }
    Logger_dbg("'name' is [%s].", name);
    
    Logger_dbg("Loop start.");
    for (index = 0; index < List_count(variables); index++)
    {
        Logger_dbg("Condition OK.");
        tmp = (DefinitionVariable)List_get(variables, index);
        Logger_dbg("tmp = %p", tmp);
        
        if (tmp->type != DEFINITION_VARIABLE_TYPE_NORMAL)
            continue;

        Logger_dbg("tmp->of.normal->name = %s", tmp->of.normal->name);
        if (wcscmp(tmp->of.normal->name, name) != 0)
            continue;
        
        definition = tmp;
        break;
    }
    Logger_dbg("Loop end.");
    
END:
    Logger_dbg("DefinitionVariable is %p", definition);
    if (definition != NULL)
    	Logger_dbg("DefinitionVariable name = '%s'", definition->of.normal->name);
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



String
DefinitionVariable_name(
	DefinitionVariable variable
)
{
    Logger_trc("[ START ]%s", __func__);
    String name = NULL;
    
    if (variable == NULL)
    {
        Logger_dbg("variable is NULL.");
        goto END;
    }
    
    if (variable->type != DEFINITION_VARIABLE_TYPE_NORMAL)
    {
        Logger_dbg("variable->type != DEFINITION_VARIABLE_TYPE_NORMAL");
        goto END;
    }
    
    if (variable->of.normal == NULL)
    {
        Logger_dbg("variable->of.normal == NULL");
        goto END;
    }
    name = variable->of.normal->name;
   
END:
    Logger_trc("[  END  ]%s", __func__);
	return name;
}



void
DefinitionVariable_set(
	DefinitionVariable variable,
	CriaId id
)
{
    if (variable->type == DEFINITION_VARIABLE_TYPE_NORMAL)
    	variable->of.normal->object = id;
    else if (variable->type == DEFINITION_VARIABLE_TYPE_ITEM)
    	Item_setObject(variable->of.item, id);
}



void*
DefinitionVariable_getObject(
	DefinitionVariable variable
)
{
    if (variable->type == DEFINITION_VARIABLE_TYPE_NORMAL)
    	return variable->of.normal->object;
    else if (variable->type == DEFINITION_VARIABLE_TYPE_ITEM)
    	return Item_getObject(variable->of.item);
    
    return NULL;    
}



DefinitionVariable
DefinitionVariable_parse(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    DefinitionVariable variable = NULL;
    String name = NULL; 
    Boolean isStatic = TRUE;
    Boolean isConstant = FALSE;
    Token token = NULL;
    Expression expression = NULL;
    CriaId value = NULL;
    
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_ATMARK)
    {
    	Logger_dbg("Static FALSE");
    	isStatic = FALSE;
    	Parser_next(parser);
	    token = Parser_getCurrent(parser);
    }
    
	
	if (token->type == TOKEN_TYPE_CONSTANT)
    {
        isConstant = TRUE;
    }
    else if (token->type == TOKEN_TYPE_IDENTIFIER)
	{
        isConstant = FALSE;
	}
    else
    {
    	Logger_err("Not field variable.");
        Parser_error(parser, token);
		goto END;
    }
    
    
    if (isStatic == FALSE && isConstant == TRUE)
    {
    	Logger_err("A constant literal can not be a instance variable.");
        Parser_error(parser, token);
		goto END;
    }
        
	
	name = token->value;
	
    
	Parser_next(parser);
	token = Parser_getCurrent(parser);
	if (token->type == TOKEN_TYPE_NEW_LINE)
		goto CREATE;
	
    
	if (token->type != TOKEN_TYPE_SUBSTITUTE)
    {
    	Logger_dbg("It may be a method.");
		goto END;
    }
    
    
	Parser_next(parser);
	token = Parser_getCurrent(parser);
	expression = Expression_parse(parser);
    if (expression == NULL)
	{
    	Logger_err("No expression.");
        Parser_error(parser, token);
		goto END;
	}
    
    value = Expression_evaluate(Parser_getInterpreter(parser), NULL, NULL, NULL, expression);
	
CREATE:
	Parser_next(parser);
    
	variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, name, isStatic, isConstant, NULL);
    if (value != NULL)
        variable->of.normal->object = value;
	
END:
	if (variable == NULL)
	{
		Logger_dbg("Not DefinitionVariable.");
		Parser_setPosition(parser, position);
	}
	
    Logger_trc("[  END  ]%s", __func__);
    return variable;
}



//==============================
//DefinitionFunction
//==============================
DefinitionFunction
DefinitionFunction_new(
    String              name,
    Boolean             isNative,
    Boolean             isStatic,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* loader
)
{
    DefinitionFunction definition = Memory_malloc(sizeof(struct DefinitionFunctionTag));
    definition->name = name;
    definition->isNative = isNative;
    definition->isStatic = isStatic;
    if (isNative == TRUE)
    {
        Logger_dbg("Native Function");
        definition->of.native.function = loader;
    }
    else
    {
        Logger_dbg("Cria Function (ParameterCount=%d, StatementCount=%d)", List_count(parameterList), List_count(statementList));
        definition->of.cria.parameterList = parameterList;
        definition->of.cria.statementList = statementList;
    }

    return definition;
}



List
DefinitionFunction_parse_parameters(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
	List parameters = List_new();
	Token token = NULL;
    DefinitionVariable variable = NULL;
	
	while (TRUE)
	{
		token = Parser_getCurrent(parser);
		if (token->type != TOKEN_TYPE_IDENTIFIER)
	    {
            if (token->type != TOKEN_TYPE_PARENTHESIS_RIGHT)
                parameters = NULL;
            
            Logger_dbg("Not identifier");
		    goto END;
	    }
	    
	    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, token->value, TRUE, FALSE, NULL);
		List_add(parameters, variable);
	    Logger_dbg("Add parameter.(%s)", DefinitionVariable_name(variable));
		
		Parser_next(parser);
		token = Parser_getCurrent(parser);
		if (token->type == TOKEN_TYPE_COMMA)
	    {
    		Parser_next(parser);
			continue;
	    }
	}
	
    Logger_dbg("parameters count = %d", List_count(parameters));
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
	return parameters;
}



DefinitionFunction
DefinitionFunction_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction functionDefinition = NULL;
    Item restore = Parser_getPosition(parser);
    Boolean isStatic = TRUE;
    String name = NULL;
    List parameters = NULL;
    List statements = NULL;
    Statement statement = NULL;
    Token token = NULL;
    Boolean isBracket = FALSE;
    wchar_t buffer[6];
    
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_ATMARK)
    {
    	isStatic = FALSE;
    	Parser_next(parser);
	    token = Parser_getCurrent(parser);
    }
    
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = token->value;
    Parser_next(parser);
    
    if (Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, FALSE) == TRUE)
    {
        isBracket = FALSE;
    }
    else if (Parser_eat(parser, TOKEN_TYPE_BRACKET_LEFT, FALSE) == TRUE)
    {
        isBracket = TRUE;
    }
    else
    {
        Logger_dbg("Not '(' or '['.");
        goto END;
    }
    
	parameters = DefinitionFunction_parse_parameters(parser);
	if (parameters == NULL)
    {
        Logger_dbg("Not parameters.");
    	goto END;
    }
	
	if (isBracket == FALSE)
    {
        Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_RIGHT, TRUE);
    }
    else if (isBracket == TRUE)
    {
        Parser_eat(parser, TOKEN_TYPE_BRACKET_RIGHT, TRUE);
    }
    else
    {
        Logger_dbg("Not ')' or ']'.");
        goto END;
    }
    
    if (isBracket == TRUE)
    {
        if (wcscmp(name, L"get") == 0 || wcscmp(name, L"set") == 0)
        {
            memset(buffer, 0x00, sizeof(buffer));
            wcscat(buffer, name);
            wcscat(buffer, L"[]");
            name = String_new(buffer);
        }
        else
        {
            Logger_dbg("Illegal function name as indexer. [%s]", String_wcsrtombs(name));
            goto END;
        }
    }
	
	if (Parser_eat(parser, TOKEN_TYPE_COLON, FALSE) == FALSE)
    {
        Logger_dbg("Not ':'.");
    	goto END;
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_NEW_LINE, TRUE) == FALSE)
    {
        Logger_dbg("Not <<NEW LINE>>.");
    	goto END;
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_INDENT, TRUE) == FALSE)
    {
        Logger_dbg("Not <<INDENT>>.");
    	goto END;
    }
	
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
	
    Logger_dbg("Create FunctionDefinition. (name=%s, parameter=%p)", name, parameters);
    functionDefinition = DefinitionFunction_new(name, FALSE, isStatic, parameters, statements, NULL);
    
END:
	if (functionDefinition == NULL)
		Parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return functionDefinition;
}



DefinitionFunction
DefinitionIndexer_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction functionDefinition = NULL;
    Item restore = Parser_getPosition(parser);
    Boolean isStatic = TRUE;
    String name = NULL;
    wchar_t buffer[6];
    List parameters = NULL;
    List statements = NULL;
    Statement statement = NULL;
    Token token = NULL;
    Boolean isGetter = FALSE;
    
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_ATMARK)
    {
    	isStatic = FALSE;
    	Parser_next(parser);
	    token = Parser_getCurrent(parser);
    }
    
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = token->value;
    
    if (wcscmp(name, L"get") == 0)
    {
        isGetter = TRUE;
    }
    else if (wcscmp(name, L"set") == 0)
    {
        isGetter = FALSE;
    }
    else
    {
        Logger_dbg("Not indexer.");
    	goto END;
    }
    
    if (Parser_eat(parser, TOKEN_TYPE_BRACKET_LEFT, FALSE) == FALSE)
    {
        Logger_dbg("Not '['.");
    	goto END;
    }
    
	parameters = DefinitionFunction_parse_parameters(parser);
	if (parameters == NULL)
    {
        Logger_dbg("Not parameters.");
    	goto END;
    }
	
	if (Parser_eat(parser, TOKEN_TYPE_BRACKET_RIGHT, FALSE) == FALSE)
    {
        Logger_dbg("Not ']'.");
    	goto END;
    }
	
	if (Parser_eat(parser, TOKEN_TYPE_COLON, FALSE) == FALSE)
    {
        Logger_dbg("Not ':'.");
    	goto END;
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_NEW_LINE, TRUE) == FALSE)
    {
        Logger_dbg("Not <<NEW LINE>>.");
    	goto END;
    }
    
	if (Parser_eat(parser, TOKEN_TYPE_INDENT, TRUE) == FALSE)
    {
        Logger_dbg("Not <<INDENT>>.");
    	goto END;
    }
	
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
	
    //Edit name
    memset(buffer, 0x00, sizeof(buffer));
    wcscat(buffer, name);
    wcscat(buffer, L"[");
    wcscat(buffer, L"]");
    Parser_next(parser);
    name = String_new(buffer);
    
    functionDefinition = DefinitionFunction_new(name, FALSE, isStatic, parameters, statements, NULL);
    
END:
	if (functionDefinition == NULL)
		Parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return functionDefinition;
}



String
DefinitionFunction_get_name(
	DefinitionFunction function
)
{
	return function->name;
}

DefinitionFunction
DefinitionFunction_search(
    List    functions,
    String  name
)
{
    int count = List_count(functions);
    int index = 0;
    DefinitionFunction definition = NULL;
    DefinitionFunction tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionFunction)(List_get(functions, index));
        if (wcscmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
DefinitionFunction_evaluate(
	Interpreter interpreter,
    CriaId object,
	List parameterList,
    CriaBlock block,
	DefinitionFunction function,
	List parameters,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    int i = 0;
    CriaId id = NULL;
    StatementResult result;
    DefinitionVariable definition = NULL;
    
    if (function->isNative == TRUE)
    {
        Logger_dbg("Call native function.(%s)", function->name);
        id = (*(function->of.native.function))(interpreter, object, parameters, block);
        goto END;
    }
    
    
    //パラメータをセット
    for (i = 0; i < List_count(parameters); i++)
    {
    	id = (CriaId)List_get(parameters, i);
    	definition = (DefinitionVariable)List_get(parameterList, i);
    	DefinitionVariable_set(definition, id);
    }

    //実行
    if (parent != NULL)
    	object = parent;

    result = Statement_executeList(interpreter, object, parameterList, block, function->of.cria.statementList);
    id = result.returns.id;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



List
DefinitionFunction_getParameterList(
	DefinitionFunction function
)
{
	return function->of.cria.parameterList;
}



//==============================
//DefinitionClass
//==============================
DefinitionClass
DefinitionClass_new(
    String name,
    Boolean isNative,
    Hash i_fields,
    Hash s_fields,
    Hash i_methods,
    Hash s_methods,
    CriaNativeClassLoader* loader
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionClass definition = NULL;
    
    if (loader != NULL)
    {
        definition = (*loader)(name);
        goto END;
    }
    
    definition = Memory_malloc(sizeof(struct DefinitionClassTag));
    definition->i_fields = i_fields;
    definition->s_fields = s_fields;
    definition->i_methods = i_methods;
    definition->s_methods = s_methods;
    definition->isNative = isNative;
    definition->name = String_new(name);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



DefinitionClass
DefinitionClass_search(
    List classList,
    String name
)
{
    int count = List_count(classList);
    int index = 0;
    DefinitionClass definition = NULL;
    DefinitionClass tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionClass)(List_get(classList, index));
        if (wcscmp(tmp->name, name) == 0)
        {
            definition = tmp;
            break;
        }
    }
    
    
    return definition;
}



CriaId
DefinitionClass_evaluate(
    Interpreter interpreter,
    CriaId  id,
    List parameterList,
    CriaBlock block,
    String  name,
    Boolean isStatic,
    DefinitionClass klass,
    List parameters
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId value = NULL;
    DefinitionFunction function = NULL;
    
    if (isStatic == TRUE)
    {
    	Hash_log_key(klass->s_methods);
    	function = Hash_get(klass->s_methods, name);
    }
    else
    {
    	Hash_log_key(klass->i_methods);
    	function = Hash_get(klass->i_methods, name);
    }
    if (function == NULL)
    {
    	Logger_err("Method is not found. (%s)/%d", name, Hash_get_count(klass->i_methods));
        Runtime_error(interpreter, "Method is not found. (%s)/%d", String_wcsrtombs(name), Hash_get_count(klass->i_methods));
    }
    
    Logger_dbg("parameters->count = %d", List_count(parameters));
    value = DefinitionFunction_evaluate(interpreter, id, parameterList, block, function, parameters, NULL);
    
    
    Logger_trc("[  END  ]%s", __func__);
    return value;
}



String
DefinitionClass_getName(
	DefinitionClass klass
)
{
	return klass->name;
}



Hash
DefinitionClass_getFields(
	DefinitionClass klass,
	Boolean isStatic
)
{
	if (isStatic == TRUE)
		return klass->s_fields;
	else
		return klass->i_fields;
}



CriaId
DefinitionClass_generateInstance(
    Interpreter interpreter,
    DefinitionClass klass,
    List parameters,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaObject object = NULL;
    DefinitionVariable variable = NULL;
    DefinitionFunction constractor = NULL;
    List fields = NULL;
    String name = NULL;
    int count = 0;
    int i = 0;
    
    
    Logger_dbg("Class name = '%s'", klass->name);
    if (klass->isNative == TRUE)
    {
    	Logger_dbg("Create object from native class.(%s)", klass->name);
    	id = DefinitionClass_evaluate(interpreter, NULL, NULL, block, L" generator ", TRUE, klass, parameters);
    }
    else
    {
    	Logger_dbg("Create object from cria class.(%s)", klass->name);
    	object = CriaObject_new(klass->name);
		fields = Hash_get_keys(DefinitionClass_getFields(klass, FALSE));
		count = List_count(fields);
		Logger_dbg("fields count = %d", count);
		for (i = 0; i < count; i++)
		{
			name = (String)List_get(fields, i);
			
			variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, name, FALSE, FALSE, NULL);
			Logger_dbg("variable->name = %s (Instance)", name);
			CriaObject_addField(object, variable);
		}
		
		id = (CriaId)object;
		Logger_dbg("id is %p, object is %p", id, object);
    }
    
    
    constractor = Hash_get(klass->s_methods, L"new");
    
    if (constractor != NULL)
    {
    	Logger_dbg("Exist constractor.");
		DefinitionFunction_evaluate(interpreter, id, DefinitionFunction_getParameterList(constractor), block, constractor, parameters, id);
    }
    

    Logger_trc("[  END  ]%s", __func__);
    return id;
}



Hash
DefinitionClass_getMethods(
	DefinitionClass klass,
	Boolean isStatic
)
{
	if (isStatic == TRUE)
		return klass->s_methods;
	else
		return klass->i_methods;
}


DefinitionClass
DefinitionClass_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionClass klass = NULL;
    DefinitionFunction function = NULL;
    DefinitionVariable variable = NULL;
    Item restore = Parser_getPosition(parser);
    String name = NULL;
    Hash i_fields = NULL;
    Hash s_fields = NULL;
    Hash i_methods = NULL;
    Hash s_methods = NULL;
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = token->value;
    Parser_next(parser);
    
    if (Parser_eat(parser, TOKEN_TYPE_COLON, FALSE) == FALSE)
    {
        Logger_dbg("Not ':'.");
    	goto END;
    }
    
    if (Parser_eat(parser, TOKEN_TYPE_NEW_LINE, FALSE) == FALSE)
    {
        Logger_dbg("Not <<NEW_LINE>>.");
    	goto END;
    }
    
    if (Parser_eat(parser, TOKEN_TYPE_INDENT, FALSE) == FALSE)
    {
        Logger_dbg("Not <<INDENT>>.");
    	goto END;
    }
    
    i_fields = Hash_new(32);
    s_fields = Hash_new(32);
    while (TRUE)
    {
    	variable = DefinitionVariable_parse(parser);
    	if (variable == NULL)
    		break;
    	
    	if (variable->type != DEFINITION_VARIABLE_TYPE_NORMAL)
            continue;
        
    	if (variable->of.normal->isStatic == TRUE)
    	{
			Logger_dbg("Add instance variable.");
			Hash_put(s_fields, DefinitionVariable_name(variable), variable);
    	}
    	else
    	{
			Logger_dbg("Add static variable.");
			Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    	}
    }
    
    i_methods = Hash_new(32);
    s_methods = Hash_new(32);
    while (TRUE)
    {
    	function = DefinitionFunction_parse(parser);
    	if (function == NULL)
    		break;
    	
    	if (function->isStatic == TRUE)
    	{
			Logger_dbg("Add instance method.");
			Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    	}
    	else
    	{
			Logger_dbg("Add static method.");
			Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    	}
    }
    
    token = Parser_getCurrent(parser);
    if (Parser_eat(parser, TOKEN_TYPE_DEDENT, FALSE) == FALSE)
    {
        Logger_dbg("Not <<DEDENT>>.");
    	goto END;
    }
    
    
    klass = DefinitionClass_new(name, FALSE, i_fields, s_fields, i_methods, s_methods, NULL);
    
END:
	if (klass == NULL)
		Parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return klass;
}

