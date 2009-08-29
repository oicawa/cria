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

 
#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "CriaString.h"
#include "Runtime.h"
#include "CriaBoolean.h"



CriaBoolean
CriaBoolean_new(
    Boolean isLiteral,
    Boolean value
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBoolean boolean = Memory_malloc(sizeof(struct CriaBooleanTag));
    
    char name[] = "Boolean";
    boolean->id.name = String_new(name);
    boolean->id.type = CRIA_DATA_TYPE_BOOLEAN;
    boolean->isLiteral = isLiteral;
    boolean->value = value;
    
    Logger_trc("[  END  ]%s", __func__);
    return boolean;
}



CriaId
CriaBoolean_operate(
    Interpreter     interpreter,
    OperationKind   kind,
    CriaBoolean     left,
    CriaBoolean     right
)
{
    Boolean leftValue = left->value;
    Boolean rightValue = right->value;
    Boolean result = FALSE;
    CriaId id = NULL;
    
    switch (kind)
    {
    case OPERATION_KIND_EQUAL:
        if (leftValue == rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    case OPERATION_KIND_NOT_EQUAL:
        if (leftValue != rightValue)
            result = TRUE;
        id = (CriaId)CriaBoolean_new(FALSE, result);
        break;
    default:
        Runtime_error(interpreter, "Not supported operation.");
        break;
    }
    
    return id;
}



CriaId
CriaBoolean_toString(
    Interpreter     interpreter,
    CriaBoolean     boolean
)
{
    String value = NULL;
    CriaString string = NULL;
    
    if (boolean->value == TRUE)
        value = String_new("true");
    else if (boolean->value == FALSE)
        value = String_new("false");
    else
        Runtime_error(interpreter, "Illegal data format as boolean.");
    
    string = CriaString_new(FALSE, value);
    
    return (CriaId)string;
}



