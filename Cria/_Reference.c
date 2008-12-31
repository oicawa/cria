#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "Expression.h"
#include "Tokenizer.h"
#include "CriaObject.h"
#include "Runtime.h"

#include "_Reference.h"



Reference
Reference_new(
    ReferenceType   type
)
{
    Reference reference = NULL;
    
    reference = Memory_malloc(sizeof(struct ReferenceTag));
    memset(reference, 0x00, sizeof(struct ReferenceTag));
    reference->type = type;
    
    return reference;
}



DefinitionVariable
ReferenceVariable_searchFromParameters(
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
ReferenceVariable_searchFromObject(
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
	
	
	definition = DefinitionVariable_search(((CriaObject)object)->fields, variableName);
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
ReferenceVariable_searchFromInterpreter(
    Interpreter interpreter,
    String variableName
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    
    definition = DefinitionVariable_search(Interpreter_variables(interpreter), variableName);
    if (definition != NULL)
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
    ReferenceVariable   variable
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    
    
	definition = ReferenceVariable_searchFromParameters(parameters, variable->name);
	if (definition != NULL)
		goto END;
    
    
	definition = ReferenceVariable_searchFromObject(object, variable->name);
	if (definition != NULL)
		goto END;
    
    
	definition = ReferenceVariable_searchFromInterpreter(interpreter, variable->name);
	if (definition != NULL)
		goto END;
    
    
    if (parameters != NULL)
    {
        definition = DefinitionVariable_new(variable->name, TRUE);
        List_add(parameters, definition);
        goto END;
    }
    
    definition = DefinitionVariable_new(variable->name, TRUE);
    List_add(Interpreter_variables(interpreter), definition);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    Logger_dbg("definition->name is '%s'", DefinitionVariable_name(definition));
    return definition;
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
    
    Token token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_COMMA)
    {
    	Token_log(token);
    	Logger_dbg("Not comma.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
	Logger_dbg("Self object's pointer.");
    
	variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    memset(variable, 0x00, sizeof(struct ReferenceVariableTag));
    variable->name = String_clone("self.");
    
    reference = Reference_new(REFERENCE_TYPE_SELF);
    reference->of.variable = variable;
	Logger_dbg("Created ReferenceVariable");
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    Token_log(token);
	Logger_dbg("Parse next 'Reference'");
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        Parser_error(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
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
    
    Token token = Parser_getCurrent(parser);
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
    memset(variable, 0x00, sizeof(struct ReferenceVariableTag));
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
    Reference   reference
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    CriaId id = NULL;
    
    switch (reference->type)
    {
    case REFERENCE_TYPE_SELF:
    	runtime_error(interpreter);
        break;
    case REFERENCE_TYPE_VARIABLE:
        definition = ReferenceVariable_evaluate(interpreter, object, parameters, reference->of.variable);
        break;
    //case REFERENCE_TYPE_FUNCTION_CALL:
    //    definition = evaluator_referenceFunctionCall(interpreter, reference->of.function);
    //    break;
    //case REFERENCE_TYPE_CLASS:
    //    break;
    default:
        break;
    }
    
    //★DefinitionVariableの階層を遡っていく処理を実行。
    if (reference->next == NULL)
    	goto END;
    	
    id = (CriaId)DefinitionVariable_getObject(definition);
    
    definition = Reference_evaluate(interpreter, id, parameters, reference->next);

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
    
    
    Token token = Parser_getCurrent(parser);
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
    memset(functionCall, 0x00, sizeof(struct ReferenceFunctionCallTag));
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



Reference
Reference_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    Reference reference = NULL;
    
    
    reference = ReferenceVariable_parse(parser);
    if (reference != NULL)
        goto END;
    
    reference = ReferenceFunctionCall_parse(parser);
    if (reference != NULL)
        goto END;
    
    //reference = reference_class(parser);
    //if (reference != null)
    //    goto END;
        
    
    Parser_setPosition(parser, position);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



