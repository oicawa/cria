#ifndef PUBLIC_CRIA_H_INCLUDED
#define PUBLIC_CRIA_H_INCLUDED

//#include "../Memory/Memory.h"
//#include "../Logger/Logger.h"

#include "_Boolean.h"
#include "_String.h"
#include "_Interpreter.h"
//#include "_DefinitionClass.h"


typedef enum
{
    INTERPRETER_COMPLE_RESULT_SUCCESS,
    INTERPRETER_COMPLE_RESULT_FAILURE,
} InterpreterCompleResult;



#define INTERPRETER_RUN_RESULT_SUCCESS      0

#define STATEMENT_PARSE_RESULT_SUCCESS      0
#define STATEMENT_PARSE_RESULT_ERROR        -1


//インスタンスへのポインタ










typedef enum
{
    ACCESS_LEVEL_PUBLIC,
    ACCESS_LEVEL_PACKAGE,
    ACCESS_LEVEL_PROTECTED,
    ACCESS_LEVEL_PRIVATE,
} AccessLevel;



#endif //PUBLIC_CRIA_H_INCLUDED
