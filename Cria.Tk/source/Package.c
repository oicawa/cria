#include <stdio.h>

#include "Interpreter.h"
#include "Definition.h"
#include "Loader.h"
#include "CriaId.h"
#include "CriaTk.h"

void
__Package_load__(Interpreter interpreter)
{
    Loader_add_class(interpreter, "TkWindow", CriaTkWindow_loadClass);
	printf ("Hello, cria native package.\n");
	return;
}
