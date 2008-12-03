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



#endif
