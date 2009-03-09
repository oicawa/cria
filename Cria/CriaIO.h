#ifndef PUBLIC_IO_H_INCLUDED
#define PUBLIC_IO_H_INCLUDED


#include "CriaId.h"
#include "List.h"
#include "Interpreter.h"


CriaId
io_write(
    Interpreter interpreter,
    CriaId object,
    List        args
);



CriaId
io_read(
    Interpreter interpreter,
    CriaId object,
    List        args
);



#endif
