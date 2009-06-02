#include <X11/X.h>  /* 一応入れとく */
#include <stdio.h>
#include <tcl.h>
#include <tk.h>
#include <string.h>

#include "Memory.h"
#include "Logger.h"

#include "CriaBoolean.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"
#include "CriaObject.h"
#include "CriaString.h"
#include "CriaInteger.h"
#include "CriaList.h"
#include "CriaBlock.h"
#include "Definition.h"
#include "CriaVariable.h"
#include "CriaTkControl.h"

#include "_CriaTk.h"



CriaId
CriaTk__generator_(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject tk = CriaObject_new("Tk");
    CriaObject_addField(tk, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "pointer", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)tk;
}



CriaId
CriaTk_new(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    interp = Tcl_CreateInterp();
    if(Tcl_Init(interp) == TCL_ERROR )
    {
        runtime_error(interpreter);
        goto END;
    }
    
    
    if(Tk_Init(interp) == TCL_ERROR )
    {
        runtime_error(interpreter);
        goto END;
    }
    
    
    CriaObject_set(interpreter, (CriaObject)object, "pointer", interp);
    
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
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    interp = CriaObject_get(interpreter, (CriaObject)object, "pointer");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return interp;
}



CriaId
CriaTk_add(
	Interpreter interpreter,
	CriaId object,
    List args
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
    	runtime_error(interpreter);
    	goto END;
    }
    

    command = Memory_malloc(sizeof(Tcl_Obj*) * args_count);
    for (i = 0; i < args_count; i++)
    {
        CriaId arg = (CriaId)List_get(args, i);
        if (arg->type != CRIA_DATA_TYPE_STRING)
        {
        	runtime_error(interpreter);
        	goto END;
        }
        CriaString string = (CriaString)arg;
        command[i] = Tcl_NewStringObj(string->value, -1);
    }
    
    
    interp = CriaTk__core_(interpreter, object);
    
    
    if (Tcl_EvalObjv(interp, args_count, command, 0) == TCL_ERROR)
    {
        runtime_error(interpreter);
        goto END;
    }
    
    
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
    
    CriaBlock_evaluate(block);
    
    Logger_trc("[  END  ]%s", __func__);
    return 0;
}


CriaId
CriaTk_bind(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaString string = NULL;
    CriaBlock block = NULL;
    Tcl_Interp *interp;
    int args_count = 0;
    
    
    args_count = List_count(args);
    if (args_count != 2)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    id = (CriaId)List_get(args, 0);
    if (id->type != CRIA_DATA_TYPE_STRING)
    {
        runtime_error(interpreter);
        goto END;
    }
    string = (CriaString)id;
    
    
    id = (CriaId)List_get(args, 1);
    if (id->type != CRIA_DATA_TYPE_BLOCK)
    {
        runtime_error(interpreter);
        goto END;
    }
    block = (CriaBlock)id;
    
    
    interp = CriaTk__core_(interpreter, object);
    
    
    Tcl_CreateCommand(interp, string->value, (Tcl_CmdProc*)CriaTk_handle_proc, (ClientData)block, NULL);
    
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaTk_main_loop(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;

    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
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
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaTk__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaTk_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);

    function = DefinitionFunction_new("add", TRUE, FALSE, NULL, NULL, CriaTk_add);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("set[]", TRUE, FALSE, NULL, NULL, CriaTk_bind);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("main_loop", TRUE, FALSE, NULL, NULL, CriaTk_main_loop);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



