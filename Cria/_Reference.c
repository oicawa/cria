#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "Expression.h"
#include "Tokenizer.h"

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
ReferenceVariable_evaluate(
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
        definition = definition_variable_search(parameters, variable->name);
        if (definition != NULL)
        {
            goto END;
        }
    }
    
    
    definition = definition_variable_search(Interpreter_variables(interpreter), variable->name);
    if (definition != NULL)
    {
        goto END;
    }
    
    //存在しなかった場合は登録
    if (parameters != NULL)
    {
        definition = definition_variable_new(variable->name);
        list_add(parameters, definition);
        goto END;
    }
    
    definition = definition_variable_new(variable->name);
    list_add(Interpreter_variables(interpreter), definition);
    
END:
    Logger_trc("[  END  ]%s", __func__);
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
    Item position = parser_getPosition(parser);
    String name = NULL;
    
    Token token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
    	Token_log(token);
    	Logger_dbg("Not identifier.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    name = Token_buffer(token);
	Logger_dbg("Variable name is '%s'", name);
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD && Token_type(token) != TOKEN_TYPE_SUBSTITUTE)
    {
    	Token_log(token);
    	Logger_dbg("Not '.' and ' = '");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
	Logger_dbg("Created ReferenceVariable");

    
	variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    memset(variable, 0x00, sizeof(struct ReferenceVariableTag));
    variable->name = string_clone(name);
    
    
    reference = Reference_new(REFERENCE_TYPE_VARIABLE);
    reference->of.variable = variable;
    
    
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
    	Token_log(token);
    	Logger_dbg("Not '.'");
        goto END;
    }
    
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    Token_log(token);
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        parser_error(token);
    
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
    
    switch (reference->type)
    {
    //case REFERENCE_TYPE_SELF:
    //    break;
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
    

    Logger_trc("[  END  ]%s", __func__);
    return definition;
}



Reference
ReferenceFunctionCall_parse(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = parser_getPosition(parser);
    Reference reference = NULL;
    ReferenceFunctionCall functionCall = NULL;
    ExpressionParameters parameters = NULL;
    String name = NULL;
    
    
    Token token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_IDENTIFIER)
    {
    	Token_log(token);
	    Logger_dbg("Not identifier.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    name = Token_buffer(token);
    Logger_dbg("function name is '%s'", name);
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_LEFT)
    {
    	Token_log(token);
	    Logger_dbg("Not '('.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    parser_next(parser);
    parameters = ExpressionParameters_parse(parser);
    if (parameters == NULL)
    {
	    Logger_dbg("Not parameters.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PARENTHESIS_RIGHT)
    {
    	Token_log(token);
	    Logger_dbg("Not ')'.");
    	parser_error(token);
        goto END;
    }
    
    functionCall = Memory_malloc(sizeof(struct ReferenceFunctionCallTag));
    memset(functionCall, 0x00, sizeof(struct ReferenceFunctionCallTag));
    functionCall->name = string_clone(name);
    functionCall->parameters = parameters;
    Logger_dbg("Created ReferenceFunctionCall");

    reference = Reference_new(REFERENCE_TYPE_FUNCTION_CALL);
    reference->of.function = functionCall;
    
    parser_next(parser);
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_PERIOD)
    {
    	Token_log(token);
	    Logger_dbg("Not '.'.");
    	goto END;
    }
    
	parser_next(parser);
	reference->next = Reference_parse(parser);
	if (reference->next == NULL)
	{
	    Logger_dbg("Not exist next Reference after '.'");
		parser_error(token);
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
    Item position = parser_getPosition(parser);
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
        
    
    parser_setPosition(parser, position);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



