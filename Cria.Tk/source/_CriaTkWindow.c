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
    
    Logger_trc("[  END  ]%s", __func__);
    return (CriaId)window;
}



CriaId
CriaTkWindow__get_field_value_(
    Interpreter interpreter,
    CriaId id,
    String field_name
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaObject object = NULL;
    CriaId field = NULL;

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
    
    
    window = (CriaObject)object;
    CriaObject_set(interpreter, window, "controls", List_new());
    CriaObject_set(interpreter, window, "caption", CriaString_new(FALSE, caption));
    CriaObject_set(interpreter, window, "width", CriaInteger_new(FALSE, width));
    CriaObject_set(interpreter, window, "height", CriaInteger_new(FALSE, height));
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return NULL;
}



CriaId
CriaTkWindow_show(
	Interpreter interpreter,
	CriaId object,
    List args
)	
{
    Logger_trc("[ START ]%s", __func__);
    CriaId id = NULL;
    CriaObject window = NULL;
    
    
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
    
    
    CriaString caption = (CriaString)CriaObject_get(interpreter, window, "caption");
    CriaInteger width = (CriaInteger)CriaObject_get(interpreter, window, "width");
    CriaInteger height = (CriaInteger)CriaObject_get(interpreter, window, "height");
    
    printf("\n----------\nCaption : [%s]\nWidth   : [%d]\nHeight  : [%d]\n\n", caption->value, width->value, height->value);
    
    


    
    
    
    Tcl_Interp *interp;           /* Tcl インタプリタ */
    char *file = "./myprog.tcl";  /* 自分のスクリプト */

    interp = Tcl_CreateInterp();  /* インタプリタを作成する */

    /* 初期化 */
    if( Tcl_Init( interp ) == TCL_ERROR ){
        fprintf( stderr, "%s\n", interp->result );
        runtime_error(interpreter);
        goto END;
    }
    if( Tk_Init( interp ) == TCL_ERROR ){
        fprintf( stderr, "Tk_Init failed:%s\n", interp->result );
        runtime_error(interpreter);
        goto END;
    }

    /* これは自分のコマンドの初期化である */
    /*
    if( MyCommand_Init( interp ) == TCL_ERROR ){
        fprintf( stderr, "Dith_Init failed:%s\n", interp->result );
        runtime_error(interpreter);
        goto END;
    }
    */

    /* 引数を適当に解釈する */
    parse_arg( argc, argv, interp );

    /* インタプリタで自分のスクリプトを実行する */
    if( Tcl_EvalFile( interp, file ) == TCL_ERROR ){
        fprintf( stderr, "%s TCL error\n", file );
        fprintf( stderr, "%s\n", interp->result );
        runtime_error(interpreter);
        goto END;
    }

    /* メインループに入り、イベントの生成を待つ */
    Tk_MainLoop();
    //return( 0 );
    
    
    
    
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return id;
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



