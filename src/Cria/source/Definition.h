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

 
#ifndef PUBLIC_DEFINITION_H_INCLUDED
#define PUBLIC_DEFINITION_H_INCLUDED


#include "Cria.h"
#include "Expression.h"
#include "Interpreter.h"
#include "List.h"
#include "CriaId.h"
#include "Parser.h"
#include "String.h"


struct DefinitionVariableNormalTag
{
    void* object;
    String name;
    Boolean isStatic;
    Boolean isConstant;
};


struct DefinitionVariableTag
{
    DefinitionVariableType type;
    union
    {
        DefinitionVariableNormal normal;
        Item item;
    } of;
};



struct DefinitionFunctionTag
{
    char* name;
    Boolean isNative;
    Boolean isStatic;
    union
    {
        struct
        {
            List parameterList;
            List statementList;
        } cria;
        struct
        {
            CriaNativeFunction* function;
        } native;
    } of;
};



struct DefinitionClassTag
{
    String name;
    Boolean isNative;
    List baseList;
    Hash i_fields;
    Hash s_fields;
    Hash i_methods;
    Hash s_methods;
};



//==============================
//DefinitionVariable
//==============================
DefinitionVariable
DefinitionVariable_search(
    List parameters,
    String name
);



String
DefinitionVariable_name(
	DefinitionVariable variable
);



void
DefinitionVariable_set(
	DefinitionVariable variable,
	CriaId id
);



void*
DefinitionVariable_getObject(
	DefinitionVariable variable
);



DefinitionVariable
DefinitionVariable_parse(
	Parser parser
);



//==============================
//DefinitionFunction
//==============================
List
DefinitionFunction_parse_parameters(
	Parser parser
);



DefinitionFunction
DefinitionFunction_parse(
    Parser parser
);



DefinitionFunction
DefinitionFunction_search(
    List    functions,
    char*   name
);



CriaId
DefinitionFunction_evaluate(
	Interpreter interpreter,
    CriaId id,
	List parameterList,
    CriaBlock block,
	DefinitionFunction function,
	List parameters,
	CriaId parent
);



List
DefinitionFunction_getParameterList(
	DefinitionFunction function
);



//==============================
//DefinitionClass
//==============================
String
DefinitionClass_getName(
	DefinitionClass klass
);



DefinitionClass
DefinitionClass_search(
    List classList,
    char* name
);



CriaId
DefinitionClass_evaluate(
    Interpreter interpreter,
    CriaId  id,
    List parameterList,
    CriaBlock block,
    char*   name,
    Boolean isStatic,
    DefinitionClass klass,
    List parameters
);



Hash
DefinitionClass_getFields(
	DefinitionClass klass,
	Boolean isStatic
);



Hash
DefinitionClass_getMethods(
	DefinitionClass klass,
	Boolean isStatic
);



DefinitionClass
DefinitionClass_parse(
    Parser parser
);



CriaId
DefinitionClass_generateInstance(
    Interpreter interpreter,
    DefinitionClass klass,
    List parameters,
    CriaBlock block
);



#endif
