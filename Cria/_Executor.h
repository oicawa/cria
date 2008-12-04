#ifndef PRIVATE_EXECUTOR_H_INCLUDED
#define PRIVATE_EXECUTOR_H_INCLUDED



#include "_Interpreter.h"
#include "_CriaId.h"
#include "_List.h"
#include "_Statement.h"



StatementResult
executor_executeStatementList(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    List        statements
);



StatementResult
executor_executeStatement(
    Interpreter interpreter,
    CriaId object, 
    List parameters,
    Statement   statement
);



void
executor_executeFunctionCallStatement(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    StatementFunctionCall statement
);



void
executor_executeSubstituteStatement(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    StatementSubstitute statement
);



#endif
