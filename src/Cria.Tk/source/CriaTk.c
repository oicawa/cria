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
    
    CriaObject tk = CriaObject_new("Tk");
    CriaObject_addField(tk, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "pointer", FALSE, FALSE, NULL));
    
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
    	Runtime_error(interpreter, "Data type of scoped object is illegal.");
    	goto END;
    }
    
    
    interp = CriaObject_get(interpreter, (CriaObject)object, "pointer");
    
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
        command[i] = Tcl_NewStringObj(string->value, -1);
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
    CriaBlock block_value = NULL;
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
    
    
    id = (CriaId)List_get(args, 1);
    if (id->type != CRIA_DATA_TYPE_BLOCK)
    {
        Runtime_error(interpreter, "Data type of 2nd argument is illegal.");
        goto END;
    }
    block_value = (CriaBlock)id;
    
    id = (CriaId)List_get(args, 2);
    
    interp = CriaTk__core_(interpreter, object);
    
    
    Tcl_CreateCommand(interp, string->value, (Tcl_CmdProc*)CriaTk_handle_proc, (ClientData)block_value, NULL);
    
    
    
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
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaTk__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaTk_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);

    function = DefinitionFunction_new("do", TRUE, FALSE, NULL, NULL, CriaTk_do);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("set[]", TRUE, FALSE, NULL, NULL, CriaTk_bind);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("main_loop", TRUE, FALSE, NULL, NULL, CriaTk_main_loop);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



