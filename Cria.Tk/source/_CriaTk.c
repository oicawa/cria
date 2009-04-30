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
#include "Definition.h"
#include "CriaVariable.h"

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
//    CriaObject_addField(tk, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "pointer", FALSE, FALSE, NULL));
    CriaObject_addField(tk, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "commands", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)tk;
}


/*
Tcl_Interp*
CriaTk__core_(
	Interpreter interpreter,
	CriaId object
)
{
    Logger_trc("[ START ]%s", __func__);
    Tcl_Interp* interp = NULL;

    
    Logger_dbg("Check object data type.");
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	runtime_error(interpreter);
    	goto END;
    }

    if (strcmp(object->name, "Tk") != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }

    interp = (Tcl_Interp*)CriaObject_get(interpreter, (CriaObject)object, "pointer");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return list;
}
*/


CriaId
CriaTk_new(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaId arg = NULL;
    List commands = NULL;
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    if (List_count(args) != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    arg = (CriaId)List_get(args, 0);
    if (arg->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("1st argument is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	runtime_error(interpreter);
    	goto END;
    }
    
    if (strcmp(arg->name, "List") != 0)
    {
    	Logger_err("1st argument is not List object.");
    	runtime_error(interpreter);
    	goto END;
    }
    

    commands = CriaList__core_(interpreter, arg);
    CriaObject_set(interpreter, (CriaObject)object, "commands", commands);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



CriaId
CriaTk_loop(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List commands = NULL;
    List command = NULL;
    int command_count = 0;
    int index = 0;
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    char size[16];
    Tcl_Obj** cmd = NULL;
    memset(size, 0x00, 16);
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    commands = (List)CriaObject_getObject(object, "commands");
    if (commands == NULL)
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
    
    command_count = List_count(commands);
    int length = 0;
    int j = 0;
    for (index = 0; index < command_count; index++)
    {
        command = CriaList__core_(interpreter, (CriaId)List_get(commands, index));
        length = List_count(command);
        cmd = Memory_malloc(length);
        for (j = 0; j < length; j++)
        {
            CriaString field = (CriaString)List_get(command, j);
            cmd[j] = Tcl_NewStringObj(field->value, -1);
        }
        if (Tcl_EvalObjv(interp, length, cmd, 0) == TCL_ERROR)
        {
            runtime_error(interpreter);
            goto END;
        }
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
    DefinitionVariable variable = NULL;
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    Hash i_fields = Hash_new(32);
    Hash s_fields = Hash_new(32);
    Hash i_methods = Hash_new(32);
    Hash s_methods = Hash_new(32);
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "commands", FALSE, FALSE, NULL);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaTk__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaTk_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("loop", TRUE, FALSE, NULL, NULL, CriaTk_loop);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



