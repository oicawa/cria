#ifndef PUBLIC_STATEMENT_H_INCLUDED
#define PUBLIC_STATEMENT_H_INCLUDED

#include "Parser.h"
#include "Expression.h"

typedef struct StatementTag *Statement;
typedef struct StatementFunctionCallTag *StatementFunctionCall;
typedef struct StatementSubstituteTag *StatementSubstitute;
typedef struct StatementReferenceTag *StatementReference;
typedef struct StatementGotoTag *StatementGoto;
typedef struct StatementIfTag *StatementIf;
typedef struct StatementWhileTag *StatementWhile;


typedef enum {
    STATEMENT_RESULT_NORMAL = 1,
    STATEMENT_RESULT_RETURN,
    STATEMENT_RESULT_BREAK,
    STATEMENT_RESULT_CONTINUE,
    STATEMENT_RESULT_LABEL,
} StatementResultType;



typedef struct {
    StatementResultType type;
    union {
        String label;
        CriaId id;
    } returns;
} StatementResult;



Statement
Statement_parse(
	Parser parser
);



StatementResult
Statement_executeList(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    List statements
);



#endif
