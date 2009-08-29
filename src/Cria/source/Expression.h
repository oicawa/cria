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

 
#ifndef PUBLIC_EXPRESSION_H_INCLUDED
#define PUBLIC_EXPRESSION_H_INCLUDED



#include "Cria.h"
#include "Parser.h"
#include "CriaId.h"
#include "String.h"
#include "Interpreter.h"
#include "Definition.h"



struct ExpressionTag
{
    ExpressionKind  kind;
    union {
        ExpressionFunctionCall      _functionCall_;
        ExpressionOperation         _operation_;
        ExpressionGenerate          _generate_;
        ExpressionVariable          _variable_;
        ExpressionReference         _reference_;
        ExpressionBlock             _block_;
    } of;
};



struct ExpressionOperationTag
{
    OperationKind   kind;
    Expression      left;
    Expression      right;
};



struct ExpressionFunctionCallTag
{
    String name;
    ExpressionParameters parameters;
    ExpressionBlock block;
};



struct ExpressionIndexerTag
{
    String                  name;
    ExpressionParameters    parameters;
};



struct ExpressionGenerateTag
{
    String                  name;
    ExpressionParameters    parameters;
};



struct ExpressionParametersTag
{
    List    list;
};



struct ExpressionStringLiteralTag
{
    String  value;
};



struct ExpressionIntegerLiteralTag
{
    int     value;
};



struct ExpressionBooleanLiteralTag
{
    Boolean value;
};



struct ExpressionVariableTag
{
    String  name;
    Boolean isStatic;
    Boolean isConstant;
};



struct ExpressionClassTag
{
    String  name;
};



struct ExpressionReferenceTag
{
    ExpressionReferenceType     type;
    union {
        ExpressionClass         klass;
        ExpressionVariable      variable;
        ExpressionFunctionCall  function;
        ExpressionIndexer       indexer;
        ExpressionGenerate      generate;
        ExpressionStringLiteral     string;
        ExpressionIntegerLiteral    integer;
        ExpressionBooleanLiteral    boolean;
    } of;
    ExpressionReference         next;
};



struct ExpressionBlockTag
{
    DefinitionFunction function;
};



CriaId
ExpressionFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    CriaBlock block,
    ExpressionFunctionCall expression,
    CriaId parent
);



CriaId
ExpressionIndexer_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    ExpressionIndexer expression,
    CriaId parent
);



CriaId
ExpressionGenerate_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameterList,
    CriaBlock block,
    ExpressionGenerate  expression
);



CriaId
ExpressionBooleanLiteral_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionBooleanLiteral expression
);



CriaId
ExpressionIntegerLiteral_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionIntegerLiteral expression
);



CriaId
ExpressionStringLiteral_evaluate(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    ExpressionStringLiteral expression
);



void*
ExpressionReference_getReference(
    ExpressionReference reference
);



ExpressionReference
ExpressionReference_parse(
	Parser parser
);



Expression
Expression_parse(
	Parser parser
);



ExpressionFunctionCall
ExpressionFunctionCall_new(
    String name,
    ExpressionParameters parameters
);



CriaId
ExpressionFunctionCall_evaluate(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    CriaBlock block,
    ExpressionFunctionCall expression,
    CriaId parent
);



ExpressionReference
ExpressionFunctionCall_parse(
    Parser parser
);



ExpressionReference
ExpressionIndexer_parse(
    Parser parser
);



CriaId
ExpressionReference_evaluate(
    Interpreter         interpreter,
    CriaId variable,
    List parameters,
    CriaBlock block,
    ExpressionReference expression,
    CriaId parent
);



ExpressionParameters
ExpressionParameters_parse(
    Parser  parser
);



CriaId
Expression_evaluate(
    Interpreter interpreter,
    CriaId object,
	List parameters,
    CriaBlock block,
    Expression  expression
);



List
ExpressionParameters_get_list(
	ExpressionParameters parameters
);



#endif
