#ifndef PUBLIC_CRIA_CORE_IO_H_INCLUDED
#define PUBLIC_CRIA_CORE_IO_H_INCLUDED


#include "CriaId.h"
#include "List.h"
#include "Interpreter.h"


CriaId
IO_write(
    Interpreter interpreter,
    CriaId object,
    List        args
);



CriaId
IO_read(
    Interpreter interpreter,
    CriaId object,
    List        args
);



#endif
