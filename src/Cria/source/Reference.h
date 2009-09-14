/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
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



struct ReferenceLiteralTag
{
    ReferenceLiteralType type;
    union {
        String string;
        long integer;
        Boolean boolean;
    } of;
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
        ReferenceLiteral literal;
    } of;
    Reference next;
};



void ReferenceFunctionCall_evaluate(Interpreter interpreter, CriaId object, List parameters, CriaBlock block, Reference reference, CriaId parent);
void ReferenceIndexer_evaluate(Interpreter interpreter, CriaId object, List parameters, CriaBlock block, Reference reference, CriaId parent);



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
    CriaBlock block,
    Reference reference,
    CriaId parent
);



//CriaId
void
ReferenceClass_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    CriaBlock block,
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
    CriaBlock block,
    Reference   reference,
    CriaId parent
);


#endif
