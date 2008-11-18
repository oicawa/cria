#include <stdio.h>

#include "_Cria.h"



void
executor_executeSubstituteStatement(
    Interpreter         interpreter,
    SubstituteStatement statement
)
{
    Logger_trc("[ START ]%s", __func__);
    VariableDefinition definition = NULL;
    Reference reference = statement->reference;
    CriaId id = NULL;
    
    //‰E•Ó‚ÌŽÀs
    id = evaluator_expression(interpreter, statement->expression);
    
    
    //¶•Ó‚ÉŠY“–‚·‚é•Ï”‚ÌŒŸõ
    definition = evaluator_reference(interpreter, reference);
    
    
    //‘ã“üˆ—
    definition->object = id;
    
    
    Logger_trc("[  END  ]%s", __func__);
}



void
executor_executeFunctionCallStatement(
    Interpreter             interpreter,
    FunctionCallStatement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    evaluator_referenceExpression(interpreter, statement->expression);
    Logger_trc("[  END  ]%s", __func__);
}



StatementResult
executor_executeIfStatement(
    Interpreter interpreter,
    IfStatement statement
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
        id = (CriaId)CriaBoolean_new(NULL, TRUE, TRUE);
    }
    else
    {
        Logger_dbg("statement->condition exists.");
        id = evaluator_expression(interpreter, statement->condition);
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
        result = executor_executeStatementList(interpreter, statement->statements);
        goto END;
    }
    
    Logger_dbg("statement->condition was 'false'.");
    if (statement->_if_ == NULL)
    {
        Logger_dbg("Exit IfStatement.");
        goto END;
    }
    
    Logger_dbg("Execute next IfStatement.");
    result = executor_executeIfStatement(interpreter, statement->_if_);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



StatementResult
executor_executeStatement(
    Interpreter interpreter,
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;

    result.type = STATEMENT_RESULT_NORMAL;
    Logger_dbg("result.type = %d", result.type);
    
    Logger_dbg("statement pointer = [%p]", statement);
    interpreter->lineNumber = statement->line;
    Logger_dbg("statement->line = %d", statement->line);

    switch (statement->kind)
    {
    case STATEMENT_KIND_SUBSTITUTE:
        executor_executeSubstituteStatement(interpreter, statement->of._substitute_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        executor_executeFunctionCallStatement(interpreter, statement->of._functionCall_);
        break;
    case STATEMENT_KIND_IF:
        result = executor_executeIfStatement(interpreter, statement->of._if_);
        break;
    /*
    case STATEMENT_KIND_WHILE:
        result = executor_executeWhileStatement(interpreter, statement->of._while_);
        break;
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
        result = executor_executeStatement(interpreter, statement);
    }
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



