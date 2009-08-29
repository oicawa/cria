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

 
#ifndef PUBLIC_STATEMENT_H_INCLUDED
#define PUBLIC_STATEMENT_H_INCLUDED

#include "Cria.h"
#include "Parser.h"
#include "Expression.h"
#include "Reference.h"
#include "String.h"
#include "CriaId.h"




struct StatementTag
{
    StatementKind kind;
    union {
        StatementFunctionCall   _functionCall_;
        StatementSubstitute     _substitute_;
        StatementReference      _reference_;
        StatementIf             _if_;
        StatementWhile          _while_;
        StatementGoto           _goto_;
    } of;
};



struct StatementFunctionCallTag
{
    ExpressionReference  expression;
};



struct StatementSubstituteTag
{
    Reference   reference;
    Expression  expression;
};



struct StatementReferenceTag
{
    Reference   reference;
};



struct StatementGotoTag
{
	GotoType type;
	union
	{
	    String  label;
	    Expression expression;
	} of;
};



struct StatementIfTag
{
    Expression  condition;
    List        statements;
    StatementIf _if_;
};



struct StatementWhileTag
{
    Expression  condition;
    List        statements;
};



Statement
Statement_new(
    StatementKind   kind
);



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
    CriaBlock block,
    List statements
);



#endif
