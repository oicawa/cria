#ifndef PUBLIC_REFERENCE_H_INCLUDED
#define PUBLIC_REFERENCE_H_INCLUDED



#include "Cria.h"
#include "String.h"
#include "Parser.h"
#include "Expression.h"
#include "Definition.h"


struct ReferenceVariableTag
{
    String  name;
    Expression value;
};



struct ReferenceIndexerTag
{
    ExpressionParameters parameters;
    Expression value;
};



struct ReferenceClassTag
{
    String  name;
};



struct ReferenceTag
{
    ReferenceType type;
    int line;
    int column;
    String file_path;
    union {
        ReferenceVariable variable;
        ExpressionFunctionCall function;
        ReferenceIndexer indexer;
        ReferenceClass klass;
    } of;
    Reference next;
};



void ReferenceFunctionCall_evaluate(Interpreter interpreter, CriaId object, List parameters, ExpressionBlock block, Reference reference, CriaId parent);
void ReferenceIndexer_evaluate(Interpreter interpreter, CriaId object, List parameters, ExpressionBlock block, Reference reference, CriaId parent);



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
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
    Reference reference,
    CriaId parent
);



//CriaId
void
ReferenceClass_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    ExpressionBlock block,
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
    ExpressionBlock block,
    Reference   reference,
    CriaId parent
);


#endif
