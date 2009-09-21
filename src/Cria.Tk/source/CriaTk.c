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

 
#include <X11/X.h>  /* 一応入れとく */
#include <stdio.h>
#include <tcl.h>
#include <tk.h>
#include <string.h>

#include "Cria.h"
#include "CriaTk.h"



CriaId
CriaTk__generator_(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject tk = CriaObject_new(L"Tk");
    CriaObject_addField(tk, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, L"pointer", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)tk;
}



CriaId
CriaTk_new(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)
{
    Logger_trc("[ START ]%s", __func__);
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	Runtime_error(interpreter, "Data type is illegal.");
    	goto END;
    }
    
    
    if (List_count(args) != 0)
    {
    	Runtime_error(interpreter, "Illegal argument count.");
    	goto END;
    }
    
    
    interp = Tcl_CreateInterp();
    if(Tcl_Init(interp) == TCL_ERROR )
    {
        Runtime_error(interpreter, "Creating Tcl/Tk interpreter is failure.");
        goto END;
    }
    
    
    if(Tk_Init(interp) == TCL_ERROR )
    {
        Runtime_error(interpreter, "Initializing Tcl/Tk interpreter is failure.");
        goto END;
    }
    
    
    CriaObject_set(interpreter, (CriaObject)object, L"pointer", interp);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



Tcl_Interp*
CriaTk__core_(
    Interpreter interpreter,
	CriaId object
)
{
    Logger_trc("[ START ]%s", __func__);
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    
    
    interp = CriaObject_get(interpreter, (CriaObject)object, L"pointer");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return interp;
}



CriaId
CriaTk_do(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    int args_count = 0;
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    char size[16];
    int i = 0;
    Tcl_Obj** command = NULL;
    memset(size, 0x00, 16);
    
    
    Logger_dbg("Check arguments count.");
    args_count = List_count(args);
    if (args_count <= 0)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    

    command = Memory_malloc(sizeof(Tcl_Obj*) * args_count);
    for (i = 0; i < args_count; i++)
    {
        CriaId arg = (CriaId)List_get(args, i);
        if (arg == NULL)
        {
        	Runtime_error(interpreter, "Data type of argument '%d' is null.", i);
        	goto END;
        }
        if (arg->type != CRIA_DATA_TYPE_STRING)
        {
        	Runtime_error(interpreter, "Data type of argument '%d' is illegal.", i);
        	goto END;
        }
        CriaString string = (CriaString)arg;
        command[i] = Tcl_NewStringObj((char*)String_wcsrtombs(string->value), -1);
    }
    
    
    interp = CriaTk__core_(interpreter, object);
    
    
    if (Tcl_EvalObjv(interp, args_count, command, 0) == TCL_ERROR)
    {
        Runtime_error(interpreter, "Evaluating Tcl/Tk script is failure.");
        goto END;
    }
    
    
    id = (CriaId)CriaString_new(FALSE, (String)Tcl_GetStringResult(interp));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}


int
CriaTk_handle_proc(
    ClientData data,
    Tcl_Interp* interp,
    int argc,
    char* argv[]
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBlock block = (CriaBlock)data;
    
    CriaBlock_evaluate(block, List_new());
    
    Logger_trc("[  END  ]%s", __func__);
    return 0;
}


CriaId
CriaTk_bind(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaString string = NULL;
    Tcl_Interp *interp;
    int args_count = 0;
    
    
    args_count = List_count(args);
    
    
    id = (CriaId)List_get(args, 0);
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        Runtime_error(interpreter, "Data type of 1st argument is illegal.");
        goto END;
    }
    string = (CriaString)id;
    
    
    if (args_count == 2)
    {
        id = (CriaId)List_get(args, 1);
        if (id->type != CRIA_DATA_TYPE_BLOCK)
        {
            Runtime_error(interpreter, "Data type of 1st argument is illegal.");
            goto END;
        }
        block = (CriaBlock)id;
    }
    else if (block == NULL)
    {
        Runtime_error(interpreter, "Block is not defined.");
        goto END;
    }
    
    interp = CriaTk__core_(interpreter, object);
    
    
    Tcl_CreateCommand(interp, String_wcsrtombs(string->value), (Tcl_CmdProc*)CriaTk_handle_proc, (ClientData)block, NULL);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}


/*
CriaId
CriaTk_set_value(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaString variable = NULL;
    CriaString value = NULL;
    Tcl_Interp *interp;
    int args_count = 0;
    
    
    args_count = List_count(args);
    if (args_count != 2)
    {
        Runtime_error(interpreter, "Argument count is illegal.");
        goto END;
    }
    
    
    id = (CriaId)List_get(args, 0);
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        Runtime_error(interpreter, "Data type of 1st argument is illegal.");
        goto END;
    }
    variable = (CriaString)id;
    
    
    id = (CriaId)List_get(args, 1);
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        Runtime_error(interpreter, "Data type of 2nd argument is illegal.");
        goto END;
    }
    value = (CriaString)id;
    
    
    interp = CriaTk__core_(interpreter, object);
    
    
    Tcl_SetValue(interp, String_wcsrtombs(variable->value), String_wcsrtombs(value->value));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    id = NULL;
    return id;
}
*/



CriaId
CriaTk_get_value(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaString variable = NULL;
    char* value = NULL;
    Tcl_Interp *interp;
    int args_count = 0;
    
    
    args_count = List_count(args);
    if (args_count != 1)
    {
        Runtime_error(interpreter, "Argument count is illegal.");
        goto END;
    }
    
    
    id = (CriaId)List_get(args, 0);
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        Runtime_error(interpreter, "Data type of 1st argument is illegal.");
        goto END;
    }
    variable = (CriaString)id;
    
    
    interp = CriaTk__core_(interpreter, object);
    
    
    value = Tcl_GetVar(interp, String_wcsrtombs(variable->value), TCL_GLOBAL_ONLY);
    id = (CriaId)CriaString_new(FALSE, String_mbsrtowcs(value));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}


CriaId
CriaTk_main_loop(
	Interpreter interpreter,
	CriaId object,
    List args,
    CriaBlock block
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;

    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	Runtime_error(interpreter, "Illegal arguments count.");
    	goto END;
    }
    

    Tk_MainLoop();
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



DefinitionClass
CriaTk_loadClass(
    String className
)
{
    Logger_trc("[ START ]%s", __func__);
    //DefinitionVariable variable = NULL;
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    Hash i_fields = Hash_new(32);
    Hash s_fields = Hash_new(32);
    Hash i_methods = Hash_new(32);
    Hash s_methods = Hash_new(32);
    
    //variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "command", FALSE, FALSE, NULL);
    //Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    function = DefinitionFunction_new(L" generator ", TRUE, TRUE, NULL, NULL, CriaTk__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new(L"new", TRUE, TRUE, NULL, NULL, CriaTk_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);

    function = DefinitionFunction_new(L"do", TRUE, FALSE, NULL, NULL, CriaTk_do);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new(L"command", TRUE, FALSE, NULL, NULL, CriaTk_bind);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
//    function = DefinitionFunction_new(L"set[]", TRUE, FALSE, NULL, NULL, CriaTk_set_value);
//    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new(L"get[]", TRUE, FALSE, NULL, NULL, CriaTk_get_value);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new(L"main_loop", TRUE, FALSE, NULL, NULL, CriaTk_main_loop);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



