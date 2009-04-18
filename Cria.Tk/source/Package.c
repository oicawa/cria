#include <stdio.h>

#include "Interpreter.h"
#include "Definition.h"
#include "Loader.h"
#include "CriaId.h"
#include "CriaTkWindow.h"

void
__Package_load__(Interpreter interpreter)
{
    Loader_add_class(interpreter, "Window", CriaTkWindow_loadClass);
	printf ("Hello, cria native package.\n");
	return;
}
