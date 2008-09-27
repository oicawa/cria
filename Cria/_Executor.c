#include <stdio.h>

#include "_Cria.h"

typedef enum {
    STATEMENT_RESULT_NORMAL = 1,
    STATEMENT_RESULT_RETURN,
    STATEMENT_RESULT_BREAK,
    STATEMENT_RESULT_CONTINUE,
} StatementResultType;



typedef struct {
    StatementResultType type;
    union {
        CriaObject  object;
    } returns;
} StatementResult;



void
evaluator_evaluateFunctionCallExpression(
    Interpreter             interpreter,
    List                    local,
    FunctionCallExpression  expression
)
{
    
}


void
executor_executeFunctionCallStatement(
    Interpreter             interpreter,
    List                    local,
    FunctionCallStatement   statement
)
{
    evaluator_evaluateFunctionCallExpression(interpreter, local, statement->expression);
}



StatementResult
executor_executeStatement(
    Interpreter interpreter,
    List        local,
    Statement   statement
)
{
    StatementResult result;

    result.type = STATEMENT_RESULT_NORMAL;

    switch (statement->kind) {
    /*
    case STATEMENT_KIND_SUBSTITUTE:
        executor_executeSubstituteStatement(interpreter, local, statement->of._substitute_);
        break;
    //*/
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

    return result;
}
