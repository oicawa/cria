#ifndef PRIVATE_STATEMENT_H_INCLUDED
#define PRIVATE_STATEMENT_H_INCLUDED


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





#endif
