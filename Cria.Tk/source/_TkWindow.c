#include <string.h>

#include "Memory.h"
#include "Logger.h"

#include "CriaBoolean.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"
#include "CriaObject.h"
#include "CriaInteger.h"
#include "Definition.h"
#include "CriaVariable.h"

#include "_TkWindow.h"



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
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)window;
}



CriaTkWindow
CriaTkWindow__core_(
    Interpreter interpreter,
    CriaId object
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaTkWindow window = NULL;

    
    Logger_dbg("Check object data type.");
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	runtime_error(interpreter);
    	goto END;
    }

    if (strcmp(object->name, "Window") != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }

    list = (void*)CriaObject_get(interpreter, (CriaObject)object, "pointer");
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return list;
}



CriaId
CriaList_new(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject list = NULL;
    
    
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
    

    list = (CriaObject)object;
    CriaObject_set(interpreter, list, "pointer", List_new());
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)list;
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
    
    variable = DefinitionVariable_new("controls", FALSE, FALSE);
    Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    //variable = DefinitionVariable_new("rows", FALSE, FALSE);
    //Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
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



