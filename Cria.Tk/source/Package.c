#include "Cria.h"
#include "CriaTk.h"

void
__Package_load__(Interpreter interpreter)
{
    Loader_add_class(interpreter, "Tk", CriaTk_loadClass);
    return;
}
