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
#include "Definition.h"
#include "CriaVariable.h"

#include "CriaTkCommand.h"



TkCommand
TkCommand_new(
    
	CriaObject object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject tk = CriaObject_new("Tk");
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)tk;
}



CriaTkCommand_create(
    Interpreter interpreter,
    CriaObject object
)
{
    int i = 0;
    int count = List_count(controls);
    void* control = NULL;
    for (i = 0; i < count; i++)
    {
        //Search function named " create commands " and run. Sample is bellow.
        control = (CriaObject)List_get(controls, i);
        function_pointer = CriaObject_get(interpreter, controls, " create commands ");
        command = (*(function_pointer))(interpreter, controls);
        List_add(commands, command);
    }
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
    CriaObject window = NULL;
    List controls = NULL;
    CriaString caption = NULL;
    CriaInteger width = NULL;
    CriaInteger height = NULL;
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    char size[16];
    memset(size, 0x00, 16);
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    window = (CriaObject)CriaObject_getObject(object, "Window");
    if (window == NULL)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    caption = (CriaString)CriaObject_get(interpreter, window, "caption");
    controls = (List)CriaObject_get(interpreter, window, "caption");
    width = (CriaInteger)CriaObject_get(interpreter, window, "width");
    height = (CriaInteger)CriaObject_get(interpreter, window, "height");
    
    interp = Tcl_CreateInterp();

    if(Tcl_Init(interp) == TCL_ERROR ){
        runtime_error(interpreter);
        goto END;
    }
    
    if(Tk_Init(interp) == TCL_ERROR ){
        runtime_error(interpreter);
        goto END;
    }

    sprintf(size, "%dx%d", width->value, height->value);
   
    Tcl_Obj* cmd_size[4];
    cmd_size[0] = Tcl_NewStringObj("wm", -1);
    cmd_size[1] = Tcl_NewStringObj("geometry", -1);
    cmd_size[2] = Tcl_NewStringObj(".", -1);
    cmd_size[3] = Tcl_NewStringObj(buffer, -1);
    if(Tcl_EvalObjv(interp, 4, cmd_size, 0) == TCL_ERROR){
        //fprintf(stderr, "おおっと: %s\n", Tcl_GetStringResult(interp));
        runtime_error(interpreter);
        goto END;
    }
    Tcl_Obj* cmd_title[4];
    cmd_title[0] = Tcl_NewStringObj("wm", -1);
    cmd_title[1] = Tcl_NewStringObj("title", -1);
    cmd_title[2] = Tcl_NewStringObj(".", -1);
    cmd_title[3] = Tcl_NewStringObj(caption->value, -1);
    if(Tcl_EvalObjv(interp, 4, cmd_title, 0) == TCL_ERROR){
        //fprintf(stderr, "おおっと: %s\n", Tcl_GetStringResult(interp));
        runtime_error(interpreter);
        goto END;
    }
    
    ShowControls(interpreter, interp, ".", controls);

    Tk_MainLoop();
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



