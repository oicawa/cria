#ifndef PUBLIC_EXECUTOR_H_INCLUDED
#define PUBLIC_EXECUTOR_H_INCLUDED



#include "Interpreter.h"
#include "CriaId.h"
#include "List.h"
#include "Statement.h"



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
