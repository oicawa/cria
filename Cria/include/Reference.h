#ifndef PUBLIC_REFERENCE_H_INCLUDED
#define PUBLIC_REFERENCE_H_INCLUDED



#include "String.h"
#include "Parser.h"
#include "Expression.h"
#include "Definition.h"



typedef struct  ReferenceTag                    *Reference;
typedef struct  ReferenceVariableTag            *ReferenceVariable;
typedef struct  ReferenceFunctionCallTag        *ReferenceFunctionCall;
typedef struct  ReferenceIndexerTag             *ReferenceIndexer;
typedef struct  ReferenceClassTag               *ReferenceClass;



typedef enum
{
	REFERENCE_TYPE_SELF,
    REFERENCE_TYPE_VARIABLE,
    REFERENCE_TYPE_FUNCTION_CALL,
    REFERENCE_TYPE_INDEXER,
    REFERENCE_TYPE_CLASS,
} ReferenceType;



String
Reference_file_path(
    Reference reference
);



int
Reference_line(
    Reference reference
);



int
Reference_column(
    Reference reference
);



//DefinitionVariable
void
ReferenceVariable_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    Reference reference,
    CriaId parent
);



//CriaId
void
ReferenceClass_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    Reference reference,
    CriaId parent
);



Reference
ReferenceFunctionCall_parse(
	Parser parser
);



Reference
Reference_parse(
	Parser parser
);



Reference
Reference_new(
    ReferenceType   type
);



void
Reference_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    Reference   reference,
    CriaId parent
);


#endif
