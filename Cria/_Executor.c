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
executor_executeStatement(
    Interpreter interpreter,
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    StatementResult result;

    result.type = STATEMENT_RESULT_NORMAL;
    
    interpreter->lineNumber = statement->line;

    switch (statement->kind)
    {
    case STATEMENT_KIND_SUBSTITUTE:
        executor_executeSubstituteStatement(interpreter, statement->of._substitute_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        executor_executeFunctionCallStatement(interpreter, statement->of._functionCall_);
        break;
    /*
    case STATEMENT_KIND_IF:
        result = executor_executeIfStatement(interpreter, statement->of._if_);
        break;
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
        perror("Not supported statement.");
        exit(1);
    }

    Logger_trc("[  END  ]%s", __func__);
    return result;
}
