#include <string.h>

#include "Memory.h"
#include "Logger.h"

#include "CriaBoolean.h"
#include "Runtime.h"
#include "String.h"
#include "StringBuffer.h"
#include "CriaObject.h"
#include "CriaInteger.h"

#include "_CriaList.h"


CriaId
CriaList__generator_(
	Interpreter interpreter,
	CriaId object,
    List args
)
{
    Logger_trc("[ START ]%s", __func__);
    
    CriaObject list = CriaObject_new("List");
    CriaObject_addField(list, DefinitionVariable_new("pointer", FALSE, FALSE));
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)list;
}



List
CriaList__core_(
	Interpreter interpreter,
	CriaId object
)
{
    Logger_trc("[ START ]%s", __func__);
    List list = NULL;

    
    Logger_dbg("Check object data type.");
    if (object->type != CRIA_DATA_TYPE_CRIA_OBJECT)
    {
    	runtime_error(interpreter);
    	goto END;
    }

    if (strcmp(object->name, "List") != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }

    list = (List)CriaObject_get(interpreter, (CriaObject)object, "pointer");
    
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



CriaId
CriaList_add(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId arg = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    arg = (CriaId)(List_get(args, 0));
    
    
    List_add(pointer, arg);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_delete(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId arg = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    arg = (CriaId)(List_get(args, 0));
    if (arg->type != CRIA_DATA_TYPE_INTEGER)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    List_delete(pointer, ((CriaInteger)arg)->value);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_insert(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId index = NULL;
    CriaId target = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 2)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    index = (CriaId)(List_get(args, 0));
    if (index->type != CRIA_DATA_TYPE_INTEGER)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    target = (CriaId)(List_get(args, 1));
    
    List_insert(pointer, ((CriaInteger)index)->value, target);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_get(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;
    CriaId index = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 1)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    index = (CriaId)(List_get(args, 0));
    if (index->type != CRIA_DATA_TYPE_INTEGER)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    id = (CriaId)List_get(pointer, ((CriaInteger)index)->value);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



CriaId
CriaList_count(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    List pointer = NULL;

    
    pointer = CriaList__core_(interpreter, object);
    
    
    Logger_dbg("Check arguments count.");
    if (List_count(args) != 0)
    {
    	runtime_error(interpreter);
    	goto END;
    }
    
    
    id = (CriaId)CriaInteger_new(FALSE, List_count(pointer));

END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
}



DefinitionClass
CriaList_loadClass(
    String className
)
{
    Logger_trc("[ START ]%s", __func__);
//    DefinitionVariable variable = NULL;
    DefinitionFunction function = NULL;
    DefinitionClass klass = NULL;

    Hash i_fields = Hash_new(32);
    Hash s_fields = Hash_new(32);
    Hash i_methods = Hash_new(32);
    Hash s_methods = Hash_new(32);
    
    //variable = DefinitionVariable_new("file", FALSE, FALSE);
    //Hash_put(i_fields, DefinitionVariable_name(variable), variable);
    
    function = DefinitionFunction_new(" generator ", TRUE, TRUE, NULL, NULL, CriaList__generator_);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("new", TRUE, TRUE, NULL, NULL, CriaList_new);
    Hash_put(s_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("add", TRUE, FALSE, NULL, NULL, CriaList_add);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("delete", TRUE, FALSE, NULL, NULL, CriaList_delete);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("insert", TRUE, FALSE, NULL, NULL, CriaList_insert);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("get", TRUE, FALSE, NULL, NULL, CriaList_get);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    function = DefinitionFunction_new("count", TRUE, FALSE, NULL, NULL, CriaList_count);
    Hash_put(i_methods, DefinitionFunction_get_name(function), function);
    
    
    klass = DefinitionClass_new(className, TRUE, i_fields, s_fields, i_methods, s_methods, NULL);

    Logger_trc("[  END  ]%s", __func__);
    return klass;
}


