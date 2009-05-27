#include <stdio.h>

#include "Interpreter.h"
//#include "Definition.h"
#include "Loader.h"
#include "CriaTk.h"
#include "CriaTkControl.h"

void
__Package_load__(Interpreter interpreter)
{
    Loader_add_class(interpreter, "Tk", CriaTk_loadClass);
    Loader_add_class(interpreter, "TkControl", CriaTkControl_loadClass);
	return;
}
