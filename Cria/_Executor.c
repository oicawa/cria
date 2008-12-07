#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "DefinitionVariable.h"
#include "Expression.h"
#include "CriaBoolean.h"
#include "Evaluator.h"
#include "Runtime.h"

#include "_Executor.h"



void
executor_executeFunctionCallStatement(
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



StatementResult
executor_executeIfStatement(
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
        id = evaluator_expression(interpreter, object, parameters, statement->condition);
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
        result = executor_executeStatementList(interpreter, object, parameters, statement->statements);
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
    result = executor_executeIfStatement(interpreter, object, parameters, statement->_if_);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



StatementResult
executor_executeWhileStatement(
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
		id = evaluator_expression(interpreter, object, parameters, statement->condition);
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
		result = executor_executeStatementList(interpreter, object, parameters, statement->statements);
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



StatementResult
executor_executeGotoStatement(
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
    		result.returns.id = evaluator_expression(interpreter, object, parameters, statement->of.expression);
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



StatementResult
executor_executeStatement(
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
    interpreter->row = statement->line;
    Logger_dbg("statement->line = %d", statement->line);

    switch (statement->kind)
    {
    case STATEMENT_KIND_SUBSTITUTE:
        executor_executeSubstituteStatement(interpreter, object, parameters, statement->of._substitute_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        executor_executeFunctionCallStatement(interpreter, object, parameters, statement->of._functionCall_);
        break;
    case STATEMENT_KIND_IF:
        result = executor_executeIfStatement(interpreter, object, parameters, statement->of._if_);
        break;
    case STATEMENT_KIND_WHILE:
        result = executor_executeWhileStatement(interpreter, object, parameters, statement->of._while_);
        break;
    case STATEMENT_KIND_GOTO:
        result = executor_executeGotoStatement(interpreter, object, parameters, statement->of._goto_);
        break;
    /*
    case STATEMENT_KIND_FOR:
        result = executor_executeForStatement(interpreter, statement->of._for_);
        break;
    case STATEMENT_KIND_RETURN:
        result = executor_executeReturnStatement(interpreter, statement->of._return_);
        break;
    case BREAK_STATEMENT:
        result = executor_executeBreakStatement(interpreter, statement->of._break_);
        break;
    case CONTINUE_STATEMENT:
        result = executor_executeContinueStatement(interpreter, statement->of._continue_);
        break;
    case STATEMENT_KIND_CATCH:
        result = executor_executeCatchStatement(interpreter, statement->of._catch_);
        break;
    case STATEMENT_KIND_FINALLY:
        result = executor_executeFinallyStatement(interpreter, statement->of._finally_);
        break;
    case STATEMENT_TYPE_COUNT_PLUS_1:   //FALLTHRU
    //*/
    default:
        Logger_err("Not supported statement.");
        runtime_error(interpreter);
    }

    Logger_trc("[  END  ]%s", __func__);
    return result;
}



StatementResult
executor_executeStatementList(
    Interpreter interpreter,
    CriaId object,
    List		parameters,
    List        statements
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;
    
    int count = statements->count;
    int index = 0;
    
    for (index = 0; index < count; index++)
    {
        Statement statement = (Statement)(list_get(statements, index));
        result = executor_executeStatement(interpreter, object, parameters, statement);
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



