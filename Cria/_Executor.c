#include <stdio.h>

#include "_Cria.h"


/*
void
executor_executeSubstituteStatement(
    Interpreter         interpreter,
    List                local,
    SubstituteStatement statement
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId variable = NULL;
    CriaId id = NULL;
    
    
    id = evaluator_evaluateExpression(interpreter, local, statement->right);
    
    
    Logger_trc("[  END  ]%s", __func__);
}
*/


void
executor_executeFunctionCallStatement(
    Interpreter             interpreter,
    List                    local,
    FunctionCallStatement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    evaluator_reference(interpreter, local, statement->expression);
    Logger_trc("[  END  ]%s", __func__);
}



StatementResult
executor_executeStatement(
    Interpreter interpreter,
    List        local,
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;

    result.type = STATEMENT_RESULT_NORMAL;

    switch (statement->kind)
    {
    case STATEMENT_KIND_SUBSTITUTE:
        //executor_executeSubstituteStatement(interpreter, local, statement->of._substitute_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        executor_executeFunctionCallStatement(interpreter, local, statement->of._functionCall_);
        break;
    /*
    case STATEMENT_KIND_IF:
        result = executor_executeIfStatement(interpreter, local, statement->of._if_);
        break;
    case STATEMENT_KIND_WHILE:
        result = executor_executeWhileStatement(interpreter, local, statement->of._while_);
        break;
    case STATEMENT_KIND_FOR:
        result = executor_executeForStatement(interpreter, local, statement->of._for_);
        break;
    case STATEMENT_KIND_RETURN:
        result = executor_executeReturnStatement(interpreter, local, statement->of._return_);
        break;
    case BREAK_STATEMENT:
        result = executor_executeBreakStatement(interpreter, local, statement->of._break_);
        break;
    case CONTINUE_STATEMENT:
        result = executor_executeContinueStatement(interpreter, local, statement->of._continue_);
        break;
    case STATEMENT_KIND_CATCH:
        result = executor_executeCatchStatement(interpreter, local, statement->of._catch_);
        break;
    case STATEMENT_KIND_FINALLY:
        result = executor_executeFinallyStatement(interpreter, local, statement->of._finally_);
        break;
    case STATEMENT_TYPE_COUNT_PLUS_1:   //FALLTHRU
    //*/
    default:
        perror("Not supported statement.");
        exit(1);
    }

    Logger_trc("[  END  ]%s", __func__);
    return result;
}
