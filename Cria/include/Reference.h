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
    REFERENCE_TYPE_CLASS,
} ReferenceType;



DefinitionVariable
ReferenceVariable_evaluate(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ReferenceVariable   variable,
    CriaId parent
);



CriaId
ReferenceClass_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ReferenceClass klass,
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



DefinitionVariable
Reference_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    Reference   reference,
    CriaId parent
);



#endif
