#include <stdio.h>

#include "Interpreter.h"
#include "Definition.h"
#include "Loader.h"
#include "CriaId.h"
#include "IO.h"

void
__Package_load__(Interpreter interpreter)
{
	Loader_add_function(interpreter, "write", IO_write);
	Loader_add_function(interpreter, "read", IO_read);
	printf ("Hello, cria native package.\n");
	return;
}
