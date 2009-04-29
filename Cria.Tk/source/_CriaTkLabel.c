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

#include "_CriaTkLabel.h"



CriaId
CriaTkLabel__generator_(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject label = CriaObject_new("Label");
    CriaObject_addField(label, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "text", FALSE, FALSE, NULL));
    CriaObject_addField(label, DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, " create commands ", FALSE, FALSE, NULL));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)label;
}



void*
CriaTkLabel__get_field_value_(
    Interpreter interpreter,
    CriaObject object,
    String field_name
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject object = NULL;
    void* field = NULL;

    field = CriaObject_get(interpreter, (CriaObject)object, field_name);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return field;
}



CriaId
CriaTkLabel_new(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject label = NULL;
    CriaId tmp = NULL;
    String text = NULL;
    
    
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
    
    
    tmp = (CriaId)List_get(args, 0);
    if (tmp->type != CRIA_DATA_TYPE_STRING)
    {
    	Logger_err("1st argument is not 'CRIA_DATA_TYPE_STRING'.");
    	runtime_error(interpreter);
    	goto END;
    }
    text = String_clone(((CriaString)tmp)->value);
    
    
    label = (CriaObject)object;
    CriaObject_set(interpreter, label, "text", CriaString_new(FALSE, text));
    CriaObject_set(interpreter, label, " create commands ", create_commands);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



List
create_commands(
	Interpreter interpreter,
	CriaObject object
)	
{
    Logger_trc("[ START ]%s", __func__);
    String text = NULL;
    char buffer[16];
    
    if (object == NULL)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    text = (CriaString)CriaObject_get(interpreter, object, "text");
    
    
    //TODO: set parent control path.
    memset(control_name, 0x00, 16);
    sprintf(control_name, ".label%p", object);
    

    List commands = List_new();
    List_add(commands, String_new("label");
    List_add(commands, String_new(control_name);
    List_add(commands, String_new("-text");
    List_add(commands, String_new(text->value);

END:
    Logger_trc("[  END  ]%s", __func__);
    return List;
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
    
    variable = DefinitionVariable_new(DEFINITION_VARIABLE_TYPE_NORMAL, "text", FALSE, FALSE, NULL);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaTkWindow__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaTkWindow_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}



