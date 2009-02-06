#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "Boolean.h"
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

#include "_Definition.h"



//==============================
//DefinitionVariable
//==============================
DefinitionVariable
DefinitionVariable_new(
    String name,
    Boolean isStatic,
    Boolean isConstant
)
{
    DefinitionVariable definition = Memory_malloc(sizeof(struct DefinitionVariableTag));
    memset(definition, 0x00, sizeof(struct DefinitionVariableTag));
    definition->name = String_clone(name);
    definition->isStatic = isStatic;
    definition->isConstant = isConstant;

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
        Logger_dbg("tmp->name = %s", tmp->name);
        if (strcmp(tmp->name, name) != 0)
            continue;
        
        definition = tmp;
        break;
    }
    Logger_dbg("Loop end.");
    
END:
    Logger_dbg("DefinitionVariable is %p", definition);
    if (definition != NULL)
    	Logger_dbg("DefinitionVariable name = '%s'", definition->name);
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



String
DefinitionVariable_name(
	DefinitionVariable variable
)
{
	return variable->name;
}



void
DefinitionVariable_set(
	DefinitionVariable variable,
	CriaId id
)
{
	variable->object = id;
}



void*
DefinitionVariable_getObject(
	DefinitionVariable variable
)
{
	return variable->object;
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
    if (Token_type(token) == TOKEN_TYPE_ATMARK)
    {
    	Logger_dbg("Static FALSE");
    	isStatic = FALSE;
    	Parser_next(parser);
	    token = Parser_getCurrent(parser);
    }
    
	
	if (Token_type(token) == TOKEN_TYPE_CONSTANT)
    {
        isConstant = TRUE;
    }
    else if (Token_type(token) == TOKEN_TYPE_IDENTIFIER)
	{
        isConstant = FALSE;
	}
    else
    {
    	Logger_err("Not field variable.");
        Parser_error(token);
		goto END;
    }
    
    
    if (isStatic == FALSE && isConstant == TRUE)
    {
    	Logger_err("A constant literal can not be a instance variable.");
        Parser_error(token);
		goto END;
    }
        
	
	name = Token_buffer(token);
	
    
	Parser_next(parser);
	token = Parser_getCurrent(parser);
	if (Token_type(token) == TOKEN_TYPE_NEW_LINE)
		goto CREATE;
	
    
	if (Token_type(token) != TOKEN_TYPE_SUBSTITUTE)
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
        Parser_error(token);
		goto END;
	}
    
    value = Expression_evaluate(Parser_getInterpreter(parser), NULL, NULL, expression);
	
CREATE:
	Parser_next(parser);
    
	variable = DefinitionVariable_new(name, isStatic, isConstant);
    if (value != NULL)
        variable->object = value;
	
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
    char*               name,
    Boolean             isNative,
    Boolean             isStatic,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* loader
)
{
    DefinitionFunction definition = Memory_malloc(sizeof(struct DefinitionFunctionTag));
    memset(definition, 0x00, sizeof(struct DefinitionFunctionTag));
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
	
	while (TRUE)
	{
		token = Parser_getCurrent(parser);
		if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
	    {
	        Logger_dbg("Not identifier");
			break;
	    }
	    
		DefinitionVariable variable = DefinitionVariable_new(Token_buffer(token), TRUE, FALSE);
		List_add(parameters, variable);
	    Logger_dbg("Add parameter.(%s)", DefinitionVariable_name(variable));
		
		Parser_next(parser);
		token = Parser_getCurrent(parser);
		if (Token_type(token) != TOKEN_TYPE_COMMA)
	    {
	        Logger_dbg("Not ', '");
			break;
	    }
	    
		Parser_next(parser);
	}
	
    Logger_dbg("parameters count = %d", List_count(parameters));
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
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_ATMARK)
    {
    	isStatic = FALSE;
    	Parser_next(parser);
	    token = Parser_getCurrent(parser);
    }
    
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = Token_buffer(token);
    Parser_next(parser);
    
    if (Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_LEFT, FALSE) == FALSE)
    {
        Logger_dbg("Not '('.");
    	goto END;
    }
    
	parameters = DefinitionFunction_parse_parameters(parser);
	if (parameters == NULL)
    {
        Logger_dbg("Not parameters.");
    	goto END;
    }
	
	if (Parser_eat(parser, TOKEN_TYPE_PARENTHESIS_RIGHT, FALSE) == FALSE)
    {
        Logger_dbg("Not ')'.");
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
        if (Token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            List_dispose(statements);
            Parser_error(token);
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
    char*   name
)
{
    int count = List_count(functions);
    int index = 0;
    DefinitionFunction definition = NULL;
    DefinitionFunction tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionFunction)(List_get(functions, index));
        if (strcmp(tmp->name, name) == 0)
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
        id = (*(function->of.native.function))(interpreter, object, parameters);
        goto END;
    }
    
    Logger_dbg("function is %p", function);
    Logger_dbg("function->name is '%s'", function->name);
    Logger_dbg("function->of.cria.parameterList is %p", function->of.cria.parameterList);
    Logger_dbg("function->of.cria.parameterList->count is %d", List_count(function->of.cria.parameterList));
    Logger_dbg("parameters->count is %d", List_count(parameters));
    
    
    //パラメータをセット
    for (i = 0; i < List_count(parameters); i++)
    {
    	id = (CriaId)List_get(parameters, i);
    	definition = (DefinitionVariable)List_get(function->of.cria.parameterList, i);
    	DefinitionVariable_set(definition, id);
    }
    
    //実行
    if (parent != NULL)
    	object = parent;
    Logger_dbg("object is %p", object);
	result = Statement_executeList(interpreter, object, parameterList, function->of.cria.statementList);
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
    char* name,
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
        Logger_dbg("Load Native Class ('%s')", name);
        Logger_dbg("Load Native Class constractor is %p", loader);
        Logger_dbg("Load Native Class constractor is *%p", *loader);
        definition = (*loader)(name);
        goto END;
    }
    
    Logger_dbg("Cria Function (Field  instance=%d, static=%d)", Hash_get_count(i_fields), Hash_get_count(s_fields));
    Logger_dbg("Cria Function (Method instance=%d, static=%d)", Hash_get_count(i_methods), Hash_get_count(s_methods));
    definition = Memory_malloc(sizeof(struct DefinitionClassTag));
    memset(definition, 0x00, sizeof(struct DefinitionClassTag));
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
    char* name
)
{
    int count = List_count(classList);
    int index = 0;
    DefinitionClass definition = NULL;
    DefinitionClass tmp = NULL;
    
    for (index = 0; index < count; index++)
    {
        tmp = (DefinitionClass)(List_get(classList, index));
        if (strcmp(tmp->name, name) == 0)
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
    char*   name,
    Boolean isStatic,
    DefinitionClass klass,
    List parameters
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId value = NULL;
    DefinitionFunction function = NULL;
    
    Logger_dbg("Method name is '%s'", name);
    Logger_dbg("klass is '%p'", klass);
    Logger_dbg("klass->i_methods is '%p'", klass->i_methods);
    Logger_dbg("klass->s_methods is '%p'", klass->s_methods);
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
        runtime_error(interpreter);
    }
    
    Logger_dbg("parameters->count = %d", List_count(parameters));
    value = DefinitionFunction_evaluate(interpreter, id, parameterList, function, parameters, NULL);
    
    
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
    List parameters
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
    	id = DefinitionClass_evaluate(interpreter, NULL, NULL, " generator ", TRUE, klass, parameters);
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
			
			variable = DefinitionVariable_new(name, FALSE, FALSE);
			Logger_dbg("variable->name = %s (Instance)", name);
			CriaObject_addField(object, variable);
		}
		
		id = (CriaId)object;
		Logger_dbg("id is %p, object is %p", id, object);
    }
    
    
    constractor = Hash_get(klass->s_methods, "new");
    
    if (constractor != NULL)
    {
    	Logger_dbg("Exist constractor.");
		DefinitionFunction_evaluate(interpreter, id, DefinitionFunction_getParameterList(constractor), constractor, parameters, id);
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
    if (Token_type(token) != TOKEN_TYPE_CLASS_LITERAL)
    {
        Logger_dbg("Not identifier.");
    	goto END;
    }
    
    name = Token_buffer(token);
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
    	
    	if (variable->isStatic == TRUE)
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
        Token_log(token);
    	goto END;
    }
    
    
    klass = DefinitionClass_new(name, FALSE, i_fields, s_fields, i_methods, s_methods, NULL);
    
END:
	if (klass == NULL)
		Parser_setPosition(parser, restore);
	
    Logger_trc("[  END  ]%s", __func__);
    return klass;
}
