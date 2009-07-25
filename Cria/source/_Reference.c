#include <string.h>

#include "Cria.h"
#include "Memory.h"
#include "Logger.h"

#include "Expression.h"
#include "Tokenizer.h"
#include "CriaObject.h"
#include "CriaClass.h"
#include "CriaVariable.h"
#include "Runtime.h"

#include "_Reference.h"



void ReferenceFunctionCall_evaluate(Interpreter interpreter, CriaId object, List parameters, ExpressionBlock block, Reference reference, CriaId parent);
void ReferenceIndexer_evaluate(Interpreter interpreter, CriaId object, List parameters, ExpressionBlock block, Reference reference, CriaId parent);



String
Reference_file_path(
    Reference reference
)
{
    return reference->file_path;
}



int
Reference_line(
    Reference reference
)
{
    return reference->line;
}



int
Reference_column(
    Reference reference
)
{
    return reference->column;
}



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



void
ReferenceVariable_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    Reference reference,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    ReferenceVariable variable = NULL;
    CriaId id = NULL;
    
    
    variable = reference->of.variable;
    
    
    if (parent != NULL)
    {
        if (parent->type == CRIA_DATA_TYPE_CRIA_OBJECT)
        {
    		definition = ReferenceVariable_evaluateFromObject(parent, variable->name);
    		if (definition != NULL)
    			goto CHECK;
    		
    		Logger_err("Field named '%s' is not found.", variable->name);
    		Runtime_error(interpreter, "Field named '%s' is not found.", variable->name);
    		goto END;
        }
        else if (parent->type == CRIA_DATA_TYPE_CRIA_CLASS)
        {
    		definition = ReferenceVariable_evaluateFromClass(interpreter, parent, variable->name);
    		if (definition != NULL)
    			goto CHECK;
    		
    		Logger_err("Field named '%s' is not found.", variable->name);
    		Runtime_error(interpreter, "Field named '%s' is not found.", variable->name);
    		goto END;
        }
        else
        {
            Runtime_error(interpreter, "Data type of parent object is not Object or Class.");
        }
    }
    
    
	definition = ReferenceVariable_evaluateFromParameters(parameters, variable->name);
	if (definition != NULL)
		goto CHECK;
	
	
	definition = ReferenceVariable_evaluateFromInterpreter(interpreter, variable->name);
	if (definition != NULL)
		goto CHECK;
	
	
	if (parameters != NULL)
	{
		definition = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, variable->name, TRUE, FALSE, NULL);
		List_add(parameters, definition);
	}
    else
    {
        definition = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, variable->name, TRUE, FALSE, NULL);
        Hash_put(Interpreter_variables(interpreter), variable->name, definition);
    }
    
CHECK:
    if (reference->next != NULL)
    {
        id = (CriaId)DefinitionVariable_getObject(definition);
        Reference_evaluate(interpreter, object, parameters, block, reference->next, id);
    }
    else if (variable->value != NULL)
    {
        id = Expression_evaluate(interpreter, object, parameters, block, variable->value);
        DefinitionVariable_set(definition, id);
    }
    else
    {
        Runtime_error(interpreter, "The value of last variable is NULL.");
        goto END;
    }
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return;
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
    if (token->type != TOKEN_TYPE_IDENTIFIER)
    {
    	Logger_dbg("Not identifier.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->value;
	Logger_dbg("Variable name is '%s'", name);
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD &&
        token->type != TOKEN_TYPE_BRACKET_LEFT &&
        token->type != TOKEN_TYPE_SUBSTITUTE)
    {
    	Logger_dbg("Not '.' and ' = '");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
	Logger_dbg("Created ReferenceVariable");

    
	variable = Memory_malloc(sizeof(struct ReferenceVariableTag));
    variable->name = String_clone(name);
    
    
    reference = Reference_new(REFERENCE_TYPE_VARIABLE);
    reference->of.variable = variable;
    
    
    if (token->type == TOKEN_TYPE_PERIOD)
    {
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        reference->next = Reference_parse(parser);
        if (reference->next == NULL)
            Parser_error(parser, token);
    }
    else if (token->type == TOKEN_TYPE_BRACKET_LEFT)
    {
        //Parser_next(parser);
    	Logger_dbg("Next Reference parse.");
        reference->next = Reference_parse(parser);
        if (reference->next == NULL)
            Parser_error(parser, token);
    }
    else if (token->type == TOKEN_TYPE_SUBSTITUTE)
    {
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        variable->value = Expression_parse(parser);
        if (variable->value == NULL)
            Parser_error(parser, token);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



void
Reference_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    Reference   reference,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    
    if (reference == NULL)
    {
        goto END;
    }
    
    Interpreter_stack_add(interpreter, reference);
    
    switch (reference->type)
    {
    case REFERENCE_TYPE_SELF:
    	reference = reference->next;
        Reference_evaluate(interpreter, object, parameters, block, reference, object);
        break;
    case REFERENCE_TYPE_VARIABLE:
        ReferenceVariable_evaluate(interpreter, object, parameters, block, reference, parent);
        break;
    case REFERENCE_TYPE_INDEXER:
        ReferenceIndexer_evaluate(interpreter, object, parameters, block, reference, parent);
        break;
    case REFERENCE_TYPE_FUNCTION_CALL:
        id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, block, reference->of.function, parent);
        Reference_evaluate(interpreter, object, parameters, block, reference->next, id);
        break;
    case REFERENCE_TYPE_CLASS:
        ReferenceClass_evaluate(interpreter, object, parameters, block, reference, parent);
        break;
    default:
        break;
    }
    
    Interpreter_stack_pop(interpreter);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}


void
ReferenceFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    Reference reference,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    ExpressionFunctionCall function = NULL;
    function = reference->of.function;
    
    id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, block, function, parent);
    
    Reference_evaluate(interpreter, object, parameters, block, reference->next, id);
    
    Logger_trc("[  END  ]%s", __func__);
    return;
}


Reference
ReferenceFunctionCall_parse(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Reference reference = NULL;
    ExpressionReference expression = NULL;
    ExpressionFunctionCall function = NULL;
    Token token = NULL;
    
    
    expression = ExpressionFunctionCall_parse(parser);
    function = (ExpressionFunctionCall)ExpressionReference_getReference(expression);
    if (function == NULL)
    {
        Logger_dbg("Not ExpressionFunctionCall.");
        goto END;
    }
    
    reference = Reference_new(REFERENCE_TYPE_FUNCTION_CALL);
    reference->of.function = function;
    
    //Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD)
    {
	    Logger_dbg("Not '.'.");
    	goto END;
    }
    
	Parser_next(parser);
	reference->next = Reference_parse(parser);
	if (reference->next == NULL)
	{
	    Logger_dbg("Not exist next Reference after '.'");
		Parser_error(parser, token);
	}
	
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



void
ReferenceIndexer_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    Reference reference,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    ReferenceIndexer indexer = NULL;
    CriaId id = NULL;
    ExpressionFunctionCall function = NULL;
    
    indexer = reference->of.indexer;

    
    if (reference->next != NULL)
    {
        //Evaluate next reference.
        function = ExpressionFunctionCall_new("get[]", indexer->parameters);
        id = ExpressionFunctionCall_evaluate(interpreter, object, parameters, block, function, parent);
        Reference_evaluate(interpreter, object, parameters, NULL, reference->next, id);
    }
    else if (indexer != NULL)
    {
        //Process function 'set[]' as substitution(?) !!add id to parameters.
        List_add(ExpressionParameters_get_list(indexer->parameters), indexer->value);
        function = ExpressionFunctionCall_new("set[]", indexer->parameters);
        ExpressionFunctionCall_evaluate(interpreter, object, parameters, block, function, parent);
    }
    else
    {
        Runtime_error(interpreter, "No next reference element, and no indexer reference.");
        goto END;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}



Reference
ReferenceIndexer_parse(
	Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Item position = Parser_getPosition(parser);
    ExpressionParameters parameters = NULL;
    Reference reference = NULL;
    ReferenceIndexer indexer = NULL;
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
    Parser_next(parser);
    
    
	indexer = Memory_malloc(sizeof(struct ReferenceIndexerTag));
    indexer->parameters = parameters;

    
    reference = Reference_new(REFERENCE_TYPE_INDEXER);
    reference->of.indexer = indexer;
    
    
    token = Parser_getCurrent(parser);
    if (token->type == TOKEN_TYPE_PERIOD ||
        token->type == TOKEN_TYPE_BRACKET_LEFT)
    {
	    Parser_next(parser);
        token = Parser_getCurrent(parser);
	    reference->next = Reference_parse(parser);
	    if (reference->next == NULL)
	    {
	        Logger_dbg("Not exist next Reference after '.'");
		    Parser_error(parser,    token);
	    }
    }
    else if (token->type == TOKEN_TYPE_SUBSTITUTE)
    {
	    Parser_next(parser);
        token = Parser_getCurrent(parser);
	    indexer->value = Expression_parse(parser);
	    if (indexer->value == NULL)
	    {
	        Logger_dbg("Not exist expression as value.");
		    Parser_error(parser, token);
	    }
    }
    else
    {
        Logger_dbg("IRegular token.");
	    Parser_error(parser, token);
    }
    
	
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



void
ReferenceClass_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    Reference reference,
    CriaId parent
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    String className = NULL;
    DefinitionClass definition = NULL;
    ReferenceClass klass = NULL;
    Hash classes = NULL;

    klass = reference->of.klass;
    
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
        Runtime_error(interpreter, "Scope object type is not 'class' or CriaObject.");
    }
    className = DefinitionClass_getName(definition);
    Logger_dbg("Real class name = '%s'.", className);
    
    
    if (definition == NULL)
    {
        Logger_err("Specified class not found. ('%s')", className);
        Runtime_error(interpreter, "Specified class not found. ('%s')", className);
        goto END;
    }
    
    
    id = (CriaId)CriaClass_new(definition);
    Reference_evaluate (interpreter, object, parameters, block, reference->next, id);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return;
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
    if (token->type == TOKEN_TYPE_CLASS)
    {
        name = token->value;
    	Logger_dbg("Class name is '%s'", name);
    }
    else if (token->type == TOKEN_TYPE_CLASS_LITERAL)
    {
        name = token->value;
    	Logger_dbg("Class name is '%s'", name);
    }
    else
    {
    	Logger_dbg("Not class or class literal.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    name = token->value;
	Logger_dbg("Class name is '%s'", name);
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    if (token->type != TOKEN_TYPE_PERIOD)
    {
    	Logger_err("Not '.'");
        Parser_error(parser, token);
        goto END;
    }
    
    
	Logger_dbg("Created ReferenceClass");

    
	klass = Memory_malloc(sizeof(struct ReferenceClassTag));
    klass->name = String_clone(name);
    
    
    reference = Reference_new(REFERENCE_TYPE_CLASS);
    reference->of.klass = klass;
    
    
    Parser_next(parser);
    token = Parser_getCurrent(parser);
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        Parser_error(parser, token);
    
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
    if (token->type != TOKEN_TYPE_PERIOD)
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
    reference->next = Reference_parse(parser);
    if (reference->next == NULL)
        Parser_error(parser, token);
    
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
    Token token = Parser_getCurrent(parser);
    int line = token->row;
    int column = token->column;
    String file_path = token->file_path;
    
    
    reference = ReferenceSelf_parse(parser);
    if (reference != NULL)
        goto STACK;
    
    reference = ReferenceVariable_parse(parser);
    if (reference != NULL)
        goto STACK;
    
    reference = ReferenceFunctionCall_parse(parser);
    if (reference != NULL)
        goto STACK;
    
    reference = ReferenceClass_parse(parser);
    if (reference != NULL)
        goto STACK;
        
    reference = ReferenceIndexer_parse(parser);
    if (reference != NULL)
        goto STACK;

    Parser_setPosition(parser, position);
    goto END;
    
    
STACK:
    reference->line = line;
    reference->column = column;
    reference->file_path = file_path;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return reference;
}



