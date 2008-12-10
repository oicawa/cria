#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "List.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "DefinitionVariable.h"
#include "Expression.h"
#include "Runtime.h"
#include "CriaBoolean.h"


#include "_Statement.h"


//==============================
//Statement
//==============================
Statement
Statement_new(
    StatementKind   kind
)
{
    Statement statement = Memory_malloc(sizeof(struct StatementTag));
    memset(statement, 0x00, sizeof(struct StatementTag));
    statement->kind = kind;
    return statement;
}



void
Statement_dispose(
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    if (statement == NULL)
    {
        goto END;
    }
    
    switch (statement->kind)
    {
    case STATEMENT_KIND_SUBSTITUTE:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_IF:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_WHILE:
        //whileStatement_dispose(statement->of._while_);
        break;
    case STATEMENT_KIND_FOR:
        //forStatement_dispose(statement->of._for_);
        break;
    case STATEMENT_KIND_GOTO:
        //returnStatement_dispose(statement->of._return_);
        break;
    case STATEMENT_KIND_CATCH:
        //finallyStatement_dispose(statement->of._catch_);
        break;
    case STATEMENT_KIND_FINALLY:
        //finallyStatement_dispose(statement->of._finally_);
        break;
    default:
        Logger_err("Illegal statement type. (%d)", statement->kind);
        break;
    }
    Memory_free(statement);
    statement = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



//==============================
//StatementWhile
//==============================
StatementResult
StatementWhile_execute(
	Interpreter interpreter,
	CriaId object,
	List parameters,
    StatementWhile statement
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;
    CriaId id = NULL;
    result.type = STATEMENT_RESULT_NORMAL;
    
    
    if ((statement == NULL) 
    	|| (statement->condition == NULL)
    	|| (statement->statements == NULL))
    {
        Logger_dbg("statement is NULL.");
        goto END;
    }
    
    while (TRUE)
    {
		id = Expression_evaluate(interpreter, object, parameters, statement->condition);
		if (id->type != CRIA_DATA_TYPE_BOOLEAN)
		{
			Logger_dbg("statement->condition was not CRIA_DATA_TYPE_BOOLEAN.");
			runtime_error(interpreter);
			goto END;
		}
		
		
		CriaBoolean boolean = (CriaBoolean)id;
		if (boolean->value == FALSE)
			break;
		
		Logger_dbg("statement->condition was 'true'.");
		result = Statement_executeList(interpreter, object, parameters, statement->statements);
		if (result.type == STATEMENT_RESULT_BREAK)
		{
			result.type = STATEMENT_RESULT_NORMAL;
			break;
		}
		
		if (result.type == STATEMENT_RESULT_CONTINUE)
			continue;
		
		if (result.type == STATEMENT_RESULT_RETURN)
			break;
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Statement
StatementWhile_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementWhile whileStatement = NULL;
    Expression condition = NULL;
    List statements = List_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_WHILE)
    {
        Logger_dbg("token type is not 'WHILE'.");
        parser_setPosition(parser, position);
        goto END;
    }
    parser_next(parser);
    
    
    condition = Expression_parse(parser);
    if (condition == NULL)
    {
        Logger_dbg("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token)  != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        Token_log(token);
        if (Token_type(token)  == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'WhileStatement'");
    whileStatement = Memory_malloc(sizeof(struct StatementWhileTag));
    memset(whileStatement, 0x00, sizeof(struct StatementWhileTag));
    whileStatement->condition = condition;
    whileStatement->statements = statements;
    
    statement = Statement_new(STATEMENT_KIND_WHILE);
    statement->of._while_ = whileStatement;
    statement->line = Token_row(token) ;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



//==============================
//StatementGoto
//==============================
StatementResult
StatementGoto_execute(
	Interpreter interpreter,
	CriaId object,
	List parameters,
    StatementGoto statement
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;
    result.type = STATEMENT_RESULT_NORMAL;
    
    
    if (statement == NULL)
    {
        Logger_dbg("statement is NULL.");
        runtime_error(interpreter);
        goto END;
    }
    
    switch (statement->type)
    {
	case GOTO_TYPE_BREAK:
		result.type = STATEMENT_RESULT_BREAK;
		break;
	case GOTO_TYPE_CONTINUE:
		result.type = STATEMENT_RESULT_CONTINUE;
		break;
	case GOTO_TYPE_LABEL:
		result.type = STATEMENT_RESULT_LABEL;
    	result.returns.label = statement->of.label;
		break;
	case GOTO_TYPE_RETURN:
		result.type = STATEMENT_RESULT_RETURN;
		if (statement->of.expression != NULL)
		{
    		result.returns.id = Expression_evaluate(interpreter, object, parameters, statement->of.expression);
    		Logger_dbg("result.returns.id->type = %d", result.returns.id->type);
		}
		break;
	default:
		runtime_error(interpreter);
		break;
    }
    
END:
    Logger_trc("[  END  ]%s(result.type=%d)", __func__, result.type);
    return result;
}



Statement
StatementGoto_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementGoto gotoStatement = NULL;
    Item position = parser_getPosition(parser);
    GotoType type = GOTO_TYPE_LABEL;
    String label = NULL;
    Expression expression = NULL;
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    switch (Token_type(token))
    {
    case TOKEN_TYPE_BREAK:
    	type = GOTO_TYPE_BREAK;
	    parser_next(parser);
        break;
    case TOKEN_TYPE_CONTINUE:
    	type = GOTO_TYPE_CONTINUE;
	    parser_next(parser);
        break;
    case TOKEN_TYPE_GOTO:
    	type = GOTO_TYPE_LABEL;
	    parser_next(parser);
	    token = parser_getCurrent(parser);
	    label = Token_buffer(token);
        break;
    case TOKEN_TYPE_RETURN:
    	type = GOTO_TYPE_RETURN;
	    parser_next(parser);
        break;
    case TOKEN_TYPE_RETURN_VALUE:
    	type = GOTO_TYPE_RETURN;
	    parser_next(parser);
	    expression = Expression_parse(parser);
        break;
    default:
    	parser_setPosition(parser, position);
    	goto END;
    }
    
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
    	parser_setPosition(parser, position);
    	parser_error(token);
    	goto END;
    }
	parser_next(parser);
    
    Logger_dbg("Create 'GotoStatement'");
    gotoStatement = Memory_malloc(sizeof(struct StatementGotoTag));
    memset(gotoStatement, 0x00, sizeof(struct StatementGotoTag));
    gotoStatement->type = type;
    if (label != NULL)
    	gotoStatement->of.label = string_clone(label);
    gotoStatement->of.expression = expression;
    
    statement = Statement_new(STATEMENT_KIND_GOTO);
    statement->of._goto_ = gotoStatement;
    statement->line = Token_row(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



//==============================
//StatementFunctionCall
//==============================
void
StatementFunctionCall_execute(
    Interpreter interpreter,
    CriaId object,
	List parameters,
    StatementFunctionCall statement
)
{
    Logger_trc("[ START ]%s", __func__);
    ExpressionReference_evaluate(interpreter, object, parameters, statement->expression);
    Logger_trc("[  END  ]%s", __func__);
}



Statement
StatementFunctionCall_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementFunctionCall  function = NULL;
    ExpressionReference expression = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    
    Logger_dbg("Check 'FunctionCallExpression'");
    expression = ExpressionReference_parse(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not reference expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_dbg("Not new line token.");
        Token_log(token);
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    parser_next(parser);
    
    
    Logger_dbg("Create 'FunctionCallStatement'");
    function = Memory_malloc(sizeof(struct StatementFunctionCallTag));
    memset(function, 0x00, sizeof(struct StatementFunctionCallTag));
    function->expression = expression;
    
    statement = Statement_new(STATEMENT_KIND_FUNCTION_CALL);
    statement->of._functionCall_ = function;
    statement->line = Token_row(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



//==============================
//StatementIf
//==============================
StatementResult
StatementIf_execute(
    Interpreter interpreter,
    CriaId object,
	List parameters,
    StatementIf statement
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;
    CriaId id = NULL;
    result.type = STATEMENT_RESULT_NORMAL;
    
    
    if (statement == NULL)
    {
        Logger_dbg("statement is NULL.");
        goto END;
    }
    
    
    if (statement->condition == NULL)
    {
        Logger_dbg("statement->condition is NULL. Create 'true'");
        id = (CriaId)CriaBoolean_new(TRUE, TRUE);
    }
    else
    {
        Logger_dbg("statement->condition exists.");
        id = Expression_evaluate(interpreter, object, parameters, statement->condition);
        if (id->type != CRIA_DATA_TYPE_BOOLEAN)
        {
            Logger_dbg("statement->condition was not CRIA_DATA_TYPE_BOOLEAN.");
            runtime_error(interpreter);
            goto END;
        }
    }
    
    
    CriaBoolean boolean = (CriaBoolean)id;
    if (boolean->value == TRUE)
    {
        Logger_dbg("statement->condition was 'true'.");
        result = Statement_executeList(interpreter, object, parameters, statement->statements);
        Logger_dbg("result.type = %d", result.type);
        goto END;
    }
    
    Logger_dbg("statement->condition was 'false'.");
    if (statement->_if_ == NULL)
    {
        Logger_dbg("Exit IfStatement.");
        goto END;
    }
    
    Logger_dbg("Execute next IfStatement.");
    result = StatementIf_execute(interpreter, object, parameters, statement->_if_);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



StatementIf
StatementIf_parseElseBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    List statements = List_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_ELSE)
    {
        Logger_err("token type is not 'ELSE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        Token_log(token);
        if (Token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is not 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct StatementIfTag));
    memset(ifStatement, 0x00, sizeof(struct StatementIfTag));
    ifStatement->statements = statements;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



StatementIf
StatementIf_parseElifBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    Expression condition = NULL;
    List statements = List_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_ELIF)
    {
        Logger_err("token type is not 'ELIF'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    condition = Expression_parse(parser);
    if (condition == NULL)
    {
        Logger_err("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        Token_log(token);
        if (Token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is not 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct StatementIfTag));
    memset(ifStatement, 0x00, sizeof(struct StatementIfTag));
    ifStatement->condition = condition;
    ifStatement->statements = statements;
    
    
    token = parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_ELSE)
    {
        Logger_dbg("token type is 'ELSE'.");
        ifStatement->_if_ = StatementIf_parseElseBlock(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



StatementIf
StatementIf_parseIfBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    Expression condition = NULL;
    List statements = List_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_IF)
    {
        Logger_dbg("token type is not 'IF'.");
        parser_setPosition(parser, position);
        goto END;
    }
    parser_next(parser);
    
    
    condition = Expression_parse(parser);
    if (condition == NULL)
    {
        Logger_dbg("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    Token_log(token);
    if (Token_type(token) != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        Token_log(token);
        if (Token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct StatementIfTag));
    memset(ifStatement, 0x00, sizeof(struct StatementIfTag));
    ifStatement->condition = condition;
    ifStatement->statements = statements;
    
    
    token = parser_getCurrent(parser);
    if (Token_type(token) == TOKEN_TYPE_ELIF)
    {
        Logger_dbg("token type is 'ELIF'.");
        ifStatement->_if_ = StatementIf_parseElifBlock(parser);
    }
    else if (Token_type(token) == TOKEN_TYPE_ELSE)
    {
        Logger_dbg("token type is 'ELSE'.");
        ifStatement->_if_ = StatementIf_parseElseBlock(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



Statement
StatementIf_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    Item position = parser_getPosition(parser);
    Token token = parser_getCurrent(parser);
    
    
    ifStatement = StatementIf_parseIfBlock(parser);
    if (ifStatement == NULL)
    {
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    statement = Statement_new(STATEMENT_KIND_IF);
    statement->of._if_ = ifStatement;
    statement->line = Token_row(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



//==============================
//StatementSubstitute
//==============================
void
StatementSubstitute_execute(
    Interpreter         interpreter,
    CriaId object,
	List parameters,
    StatementSubstitute statement
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    Reference reference = statement->reference;
    CriaId id = NULL;
    
    id = Expression_evaluate(interpreter, object, parameters, statement->expression);
    
    
    definition = Reference_evaluate(interpreter, object, parameters, reference);
    
    
    DefinitionVariable_set(definition, id);
    
    
    Logger_trc("[  END  ]%s", __func__);
}



Statement
StatementSubstitute_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementSubstitute substitute = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    Reference reference = NULL;
    Expression expression = NULL;
    
    
    Logger_dbg("Check reference expression.");
    reference = Reference_parse(parser);
    if (reference == NULL)
    {
        Logger_dbg("Not reference expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_SUBSTITUTE)
    {
        Logger_dbg("Not substitution token.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    Logger_dbg("Check 'Expression'");
    parser_next(parser);
    expression = Expression_parse(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    token = parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_dbg("Not new line token.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    parser_next(parser);
    
    
    Logger_dbg("Create 'SubstituteStatement'");
    substitute = Memory_malloc(sizeof(struct StatementSubstituteTag));
    memset(substitute, 0x00, sizeof(struct StatementSubstituteTag));
    substitute->reference = reference;
    substitute->expression = expression;
    
    statement = Statement_new(STATEMENT_KIND_SUBSTITUTE);
    statement->of._substitute_ = substitute;
    statement->line = Token_row(token);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



//==============================
//Statement
//==============================
StatementResult
Statement_execute(
    Interpreter interpreter,
    CriaId object,
    List         parameters,
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;

    result.type = STATEMENT_RESULT_NORMAL;
    Logger_dbg("result.type = %d", result.type);
    
    Logger_dbg("statement pointer = [%p]", statement);
    Interpreter_setRow(interpreter, statement->line);
    Logger_dbg("statement->line = %d", statement->line);

    switch (statement->kind)
    {
    case STATEMENT_KIND_SUBSTITUTE:
        StatementSubstitute_execute(interpreter, object, parameters, statement->of._substitute_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        StatementFunctionCall_execute(interpreter, object, parameters, statement->of._functionCall_);
        break;
    case STATEMENT_KIND_IF:
        result = StatementIf_execute(interpreter, object, parameters, statement->of._if_);
        break;
    case STATEMENT_KIND_WHILE:
        result = StatementWhile_execute(interpreter, object, parameters, statement->of._while_);
        break;
    case STATEMENT_KIND_GOTO:
        result = StatementGoto_execute(interpreter, object, parameters, statement->of._goto_);
        break;
    default:
        Logger_err("Not supported statement.");
        runtime_error(interpreter);
    }

    Logger_trc("[  END  ]%s", __func__);
    return result;
}



StatementResult
Statement_executeList(
    Interpreter interpreter,
    CriaId object,
    List		parameters,
    List        statements
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;
    
    int count = List_count(statements);
    int index = 0;
    
    for (index = 0; index < count; index++)
    {
        Statement statement = (Statement)(List_get(statements, index));
        result = Statement_execute(interpreter, object, parameters, statement);
        if (result.type != STATEMENT_RESULT_NORMAL)
        	break;
    }
    
	Logger_dbg("result.type = %d", result.type);
    if (result.type == STATEMENT_RESULT_RETURN)
    {
		Logger_dbg("result.returns.id->type = %d", result.returns.id->type);
    }
    
    
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Statement
Statement_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    
    statement = StatementSubstitute_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created SubstituteStatement.");
        goto END;
    }
    
    
    statement = StatementFunctionCall_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created FunctionCallStatement.");
        goto END;
    }
    
    
    statement = StatementIf_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created IfStatement.");
        goto END;
    }
    
    
    statement = StatementWhile_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created WhileStatement.");
        goto END;
    }
    
    
    statement = StatementGoto_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created BreakStatement.");
        goto END;
    }
    
    
    Logger_dbg("No Statement.");
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



