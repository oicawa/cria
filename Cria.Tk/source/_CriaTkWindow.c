#include <X11/X.h>  /* 一応入れとく */
#include <stdio.h>
#include <tcl.h>
#include <tk.h>
#include <string.h>

#include "Memory.h"
#include "Logger.h"

#include "CriaBoolean.h"
#include "List.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"
#include "CriaObject.h"
#include "CriaString.h"
#include "CriaInteger.h"
#include "Definition.h"
#include "CriaVariable.h"
#include "Expression.h"
#include "Definition.h"
#include "CriaTkCommand.h"

#include "_CriaTkWindow.h"



CriaId
CriaTkWindow__generator_(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject window = CriaObject_new("Window");
    CriaObject_addField(window, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "controls", FALSE, FALSE, NULL));
    CriaObject_addField(window, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "caption", FALSE, FALSE, NULL));
    CriaObject_addField(window, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "width", FALSE, FALSE, NULL));
    CriaObject_addField(window, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "height", FALSE, FALSE, NULL));
    CriaObject_addField(window, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "tk", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)window;
}



void*
CriaTkWindow__get_field_value_(
    Interpreter interpreter,
    CriaId id,
    String field_name
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject object = NULL;
    void* field = NULL;

    object = CriaObject_getObject(id, "Window");
    if (object == NULL)
    {
    	runtime_error(interpreter);
    	goto END;
    }

    field = CriaObject_get(interpreter, (CriaObject)object, field_name);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return field;
}



CriaId
CriaTkWindow_new(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject window = NULL;
    CriaId tmp = NULL;
    String caption = NULL;
    int width = 300;
    int height = 200;
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    
    
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	Logger_err("Object is not 'CRIA_DATA_TYPE_CRIA_OBJECT'.");
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    if (List_count(args) < 3)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    tmp = (CriaId)List_get(args, 0);
    if (tmp->type != CRIA_DATA_TYPE_STRING)
    {
    	Logger_err("1st argument is not 'CRIA_DATA_TYPE_STRING'.");
    	runtime_error(interpreter);
    	goto END;
    }
    caption = String_clone(((CriaString)tmp)->value);
    
    
    tmp = (CriaId)List_get(args, 1);
    if (tmp->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Logger_err("2nd argument is not 'CRIA_DATA_TYPE_INTEGER'.");
    	runtime_error(interpreter);
    	goto END;
    }
    width = ((CriaInteger)tmp)->value;
    
    
    tmp = (CriaId)List_get(args, 2);
    if (tmp->type != CRIA_DATA_TYPE_INTEGER)
    {
    	Logger_err("3rd argument is not 'CRIA_DATA_TYPE_INTEGER'.");
    	runtime_error(interpreter);
    	goto END;
    }
    height = ((CriaInteger)tmp)->value;
    
    
    interp = Tcl_CreateInterp();
    if(Tcl_Init(interp) == TCL_ERROR ){
        runtime_error(interpreter);
        goto END;
    }
    if(Tk_Init(interp) == TCL_ERROR ){
        runtime_error(interpreter);
        goto END;
    }

    
    window = (CriaObject)object;
    CriaObject_set(interpreter, window, "controls", List_new());
    CriaObject_set(interpreter, window, "tk", interp);
    CriaObject_set(interpreter, window, "caption", CriaString_new(FALSE, caption));
    CriaObject_set(interpreter, window, "width", CriaInteger_new(FALSE, width));
    CriaObject_set(interpreter, window, "height", CriaInteger_new(FALSE, height));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



List
create_commands(
    Interpreter interpreter,
    CriaObject window
)
{
    Logger_trc("[ START ]%s", __func__);
    List controls = NULL;
    CriaString caption = NULL;
    CriaInteger width = NULL;
    CriaInteger height = NULL;
    char buffer[16];
    memset(buffer, 0x00, 16);
    String field = NULL;
    List fields = NULL;
    List commands = List_new();
    TkCommand command = NULL;
    CreateCommands* function = NULL;
    
    controls = (List)CreateObject_get(interpreter, window, "controls");
    caption = (CriaString)CreateObject_get(interpreter, window, "caption");
    width = (CriaInteger)CreateObject_get(interpreter, window, "widht");
    height = (CriaInteger)CreateObject_get(interpreter, window, "height");
    
    sprintf(buffer, "%dx%d", width->value, height->value);
    
    fields = List_new();
    List_add(fields, String_new("wm"));
    List_add(fields, String_new("geometry"));
    List_add(fields, String_new("."));
    List_add(fields, String_new(buffer));
    command = TkCommand_new(TK_COMMAND_TYPE_FIELDS);
    command->fields = fields;
    List_add(commands, command);
    
    
    fields = List_new();
    List_add(fields, String_new("wm"));
    List_add(fields, String_new("title"));
    List_add(fields, String_new("."));
    List_add(fields, String_new(caption->value));
    command = TkCommand_new(TK_COMMAND_TYPE_FIELDS);
    command->fields = fields;
    List_add(commands, command);
    
    
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
    
    
    

    return List_new();
}



void
evaluate_commands(
    Interpreter interpreter,
    Tcl_Interp* interp,
    List commands
)
{
    Logger_trc("[ START ]%s", __func__);
    int count = NULL;
    int length = 0;
    int i = 0;
    int j = 0;
    TkCommand command = NULL;
    
    
    count = List_count(commands);
    
    
    for (i = 0; i < count; i++)
    {
        command = (TkCommand)List_get(commands, i);
        if (command->type == TK_COMMAND_TYPE_COMMAND)
        {
            evaluate_commands(interpreter, interp, command);
        }
        else if (command->type != TK_COMMAND_TYPE_FIELDS)
        {
            runtime_error(interpreter);
            goto END;
        }
        
        length = List_count(command->fields);
        j = 0;
        Tcl_Obj** fields = Memory_malloc(length);
        memset(fields, 0x00, length);
        for (j = 0; j < length; j++)
        {
            String field = (String)List_get(command->fields, j);
            fields[j] = Tcl_NewStringObj(field, -1);
        }
        
        if(Tcl_EvalObjv(interp, length, fields, 0) == TCL_ERROR)
        {
            //fprintf(stderr, "おおっと: %s\n", Tcl_GetStringResult(interp));
            runtime_error(interpreter);
            goto END;
        }
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaTkWindow_show(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaTkCommand command = NULL;
    
    commands = CriaTkCommand_create(interpreter, window);
    
    CriaTkCommand_evaluate(interpreter, interp, commands);
    
    Tk_MainLoop();
    
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



DefinitionClass
CriaTkWindow_loadClass(
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
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "controls", FALSE, FALSE, NULL);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "caption", FALSE, FALSE, NULL);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "width", FALSE, FALSE, NULL);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "height", FALSE, FALSE, NULL);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaTkWindow__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaTkWindow_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("show", TRUE, FALSE, NULL, NULL, CriaTkWindow_show);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    //function = DefinitionFunction_new(" indexer ", TRUE, FALSE, NULL, NULL, CriaTkWindow_getRow);
    //Hash_put(i_methods, DefinitionFunction_get_name(function), function);

    //function = DefinitionFunction_new(" indexer reference ", TRUE, FALSE, NULL, NULL, CriaTkWindow_getRowVariable);
    //Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



