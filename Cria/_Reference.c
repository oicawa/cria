#include <string.h>

#include "Memory.h"
#include "Logger.h"

#include "Expression.h"
#include "Tokenizer.h"
#include "CriaObject.h"
#include "CriaClass.h"
#include "Runtime.h"

#include "_Reference.h"



Reference
Reference_new(
    ReferenceType   type
)
{
    Reference reference = NULL;
    
    reference = Memory_malloc(sizeof(struct ReferenceTag));
    reference->type = type;
    
    return reference;
}



DefinitionVariable
ReferenceVariable_evaluateFromParameters(
    List parameters,
    String variableName
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    
	Logger_dbg("parameters is %p", parameters);
    if (parameters == NULL)
		goto END;

	Logger_dbg("Find the target field. (%s)", variableName);
	definition = DefinitionVariable_search(parameters, variableName);
	if (definition == NULL)
	{
		Logger_dbg("Not found the target variable. (%s)", variableName);
		goto END;
	}
	
	
	Logger_dbg("Found the target variable. (%s)", variableName);
	
END:    
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



DefinitionVariable
ReferenceVariable_evaluateFromObject(
    CriaId object,
    String variableName
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    
	Logger_dbg("object is %p", object);
    if (object == NULL)
    {
		Logger_dbg("The target object is NULL.");
    	goto END;
    }
	
	if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
	{
		Logger_dbg("The target object is not CRIA_DATA_TYPE_CRIA_OBJECT. (%d)", object->type);
		goto END;
	}
	
	
	Hash_log_key((((CriaObject)object)->fields));
	definition = (DefinitionVariable)Hash_get(((CriaObject)object)->fields, variableName);
	if (definition == NULL)
	{
		Logger_dbg("Not found the target field. (%s)", variableName);
		goto END;
	}
	
	
	Logger_dbg("Found the target field. (%s)", variableName);
	
END:    
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}


DefinitionVariable
ReferenceVariable_evaluateFromClass(
    Interpreter         interpreter,
    CriaId klass,
    String variableName
)
{
    Logger_trc("[ START ]%s (Variable Name = '%s')", __func__, variableName);
    DefinitionVariable variable = NULL;
    
    
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
	
	
	variable = CriaClass_getVariable(interpreter, (CriaClass)klass, variableName);
	Logger_dbg("Found the target field. (%s)", variableName);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return variable;
}



DefinitionVariable
ReferenceVariable_evaluateFromInterpreter(
    Interpreter interpreter,
    String variableName
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    
    definition = (DefinitionVariable)Hash_get(Interpreter_variables(interpreter), variableName);
    if (definition == NULL)
    {
		Logger_dbg("Not found the target global variable. (%s)", variableName);
        goto END;
    }
    
    
	Logger_dbg("Found the target global variable. (%s)", variableName);
	
END:    
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



DefinitionVariable
ReferenceVariable_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ReferenceVariable   variable,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    
    if (parent != NULL)
    {
        if (parent->type == CRIA_DATA_TYPE_CRIA_OBJECT)
        {
    		definition = ReferenceVariable_evaluateFromObject(parent, variable->name);
    		if (definition != NULL)
    			goto END;
    		
    		Logger_err("Field named '%s' is not found.", variable->name);
    		runtime_error(interpreter);
    		goto END;
        }
        else if (parent->type == CRIA_DATA_TYPE_CRIA_CLASS)
        {
    		definition = ReferenceVariable_evaluateFromClass(interpreter, parent, variable->name);
    		if (definition != NULL)
    			goto END;
    		
    		Logger_err("Field named '%s' is not found.", variable->name);
    		runtime_error(interpreter);
    		goto END;
        }
        else
        {
            runtime_error(interpreter);
        }
    }
    
    
	definition = ReferenceVariable_evaluateFromParameters(parameters, variable->name);
	if (definition != NULL)
		goto END;
	
	
	definition = ReferenceVariable_evaluateFromInterpreter(interpreter, variable->name);
	if (definition != NULL)
		goto END;
	
	
	if (parameters != NULL)
	{
		definition = DefinitionVariable_new(variable->name, TRUE, FALSE);
		List_add(parameters, definition);
		Logger_dbg("Add field named '%s' to parameters.", DefinitionVariable_name(definition));
		goto END;
	}
    
    definition = DefinitionVariable_new(variable->name, TRUE, FALSE);
    Hash_put(Interpreter_variables(interpreter), variable->name, definition);
	Logger_dbg("Add field named '%s' to interpreter as global.", DefinitionVariable_name(definition));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    Logger_dbg("definition->name is '%s'", DefinitionVariable_name(definition));
    return definition;
}



Reference
ReferenceVariable_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Reference reference = NULL;
    ReferenceVariable variable = NULL;
    Item position = Parser_getPosition(parser);
    String name = NULL;
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
    	Token_log(token);
    	Logger_dbg("Not identifier.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = Token_buffer(token);
	Logger_dbg("Variable name is '%s'", name);
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD && Token_type(token) != TOKEN_TYPE_SUBSTITUTE)
    {
    	Token_log(token);
    	Logger_dbg("Not '.' and ' = '");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
	Logger_dbg("Created ReferenceVariable");

    
	variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    variable->name = String_clone(name);
    
    
    reference = Reference_new(REFERENCE_TYPE_VARIABLE);
    reference->of.variable = variable;
    
    
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
    	Token_log(token);
    	Logger_dbg("Not '.'");
        goto END;
    }
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    Token_log(token);
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        Parser_error(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



DefinitionVariable
Reference_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    Reference   reference,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    CriaId id = NULL;
    
    switch (reference->type)
    {
    case REFERENCE_TYPE_SELF:
    	reference = reference->next;
        definition = Reference_evaluate(interpreter, object, parameters, reference, object);
        break;
    case REFERENCE_TYPE_VARIABLE:
        definition = ReferenceVariable_evaluate(interpreter, object, parameters, reference->of.variable, parent);
        break;
    //case REFERENCE_TYPE_FUNCTION_CALL:
    //    definition = evaluator_referenceFunctionCall(interpreter, reference->of.function);
    //    break;
    case REFERENCE_TYPE_CLASS:
        id = ReferenceClass_evaluate(interpreter, object, parameters, reference->of.klass, parent);
    	reference = reference->next;
        definition = Reference_evaluate(interpreter, object, parameters, reference, id);
        break;
    default:
        break;
    }
    
    //★DefinitionVariableの階層を遡っていく処理を実行。
    if (reference->next == NULL)
    	goto END;
    	
    id = (CriaId)DefinitionVariable_getObject(definition);
    
    definition = Reference_evaluate(interpreter, object, parameters, reference->next, id);

END:
	
    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



Reference
ReferenceFunctionCall_parse(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Reference reference = NULL;
    ReferenceFunctionCall functionCall = NULL;
    ExpressionParameters parameters = NULL;
    String name = NULL;
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
    	Token_log(token);
	    Logger_dbg("Not identifier.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = Token_buffer(token);
    Logger_dbg("function name is '%s'", name);
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
    	Token_log(token);
	    Logger_dbg("Not '('.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    Parser_next(parser);
    parameters = ExpressionParameters_parse(parser);
    if (parameters == NULL)
    {
	    Logger_dbg("Not parameters.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
    	Token_log(token);
	    Logger_dbg("Not ')'.");
    	Parser_error(token);
        goto END;
    }
    
    functionCall = Memory_malloc(sizeof(struct ReferenceFunctionCallTag));
    functionCall->name = String_clone(name);
    functionCall->parameters = parameters;
    Logger_dbg("Created ReferenceFunctionCall");

    reference = Reference_new(REFERENCE_TYPE_FUNCTION_CALL);
    reference->of.function = functionCall;
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
    	Token_log(token);
	    Logger_dbg("Not '.'.");
    	goto END;
    }
    
	Parser_next(parser);
	reference->next = Reference_parse(parser);
	if (reference->next == NULL)
	{
	    Logger_dbg("Not exist next Reference after '.'");
		Parser_error(token);
	}
	
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



CriaId
ReferenceClass_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ReferenceClass klass,
    CriaId parent
)
{
    Logger_trc("[ START ]%s (class name = '%s')", __func__, klass->name);
    CriaId id = NULL;
    String className = NULL;
    DefinitionClass definition = NULL;
    Hash classes = NULL;

    
    classes = Interpreter_classes(interpreter);
    if (strcmp(klass->name, "class") != 0)
    {
        definition = (DefinitionClass)Hash_get(classes, klass->name);
    }
    else if (object->type == CRIA_DATA_TYPE_CRIA_OBJECT)
    {
        definition = (DefinitionClass)Hash_get(classes, object->name);
    }
    //else if (object->type == CRIA_DATA_TYPE_CRIA_CLASS)
    //{
    //    definition = ((CriaClass)object)->definition;
    //}
    else
    {
        runtime_error(interpreter);
    }
    className = DefinitionClass_getName(definition);
    Logger_dbg("Real class name = '%s'.", className);
    
    
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



Reference
ReferenceClass_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Reference reference = NULL;
    ReferenceClass klass = NULL;
    Item position = Parser_getPosition(parser);
    String name = NULL;
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_CLASS)
    {
        name = Token_buffer(token);
    	Logger_dbg("Class name is '%s'", name);
    }
    else if (Token_type(token) == TOKEN_TYPE_CLASS_LITERAL)
    {
        name = Token_buffer(token);
    	Logger_dbg("Class name is '%s'", name);
    }
    else
    {
    	Token_log(token);
    	Logger_dbg("Not class or class literal.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = Token_buffer(token);
	Logger_dbg("Class name is '%s'", name);
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
    	Token_log(token);
    	Logger_err("Not '.'");
        Parser_error(token);
        goto END;
    }
    
    
	Logger_dbg("Created ReferenceClass");

    
	klass = Memory_malloc(sizeof(struct ReferenceClassTag));
    klass->name = String_clone(name);
    
    
    reference = Reference_new(REFERENCE_TYPE_CLASS);
    reference->of.klass = klass;
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    Token_log(token);
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        Parser_error(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
ReferenceSelf_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Reference reference = NULL;
    ReferenceVariable variable = NULL;
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
    	Logger_dbg("Not self object.");
    	Parser_setPosition(parser, position);
    	goto END;
    }
    
	Logger_dbg("Created ReferenceVariable");

    
	variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    variable->name = String_clone("_self_");
    
    
    reference = Reference_new(REFERENCE_TYPE_SELF);
    reference->of.variable = variable;
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    Token_log(token);
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        Parser_error(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



Reference
Reference_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Reference reference = NULL;
    
    
    reference = ReferenceSelf_parse(parser);
    if (reference != NULL)
        goto END;
    
    reference = ReferenceVariable_parse(parser);
    if (reference != NULL)
        goto END;
    
    reference = ReferenceFunctionCall_parse(parser);
    if (reference != NULL)
        goto END;
    
    reference = ReferenceClass_parse(parser);
    if (reference != NULL)
        goto END;
        
    
    Parser_setPosition(parser, position);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



