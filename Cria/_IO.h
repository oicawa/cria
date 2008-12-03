#ifndef PRIVATE_IO_H_INCLUDED
#define PRIVATE_IO_H_INCLUDED


#include "_CriaId.h"
#include "_List.h"
#include "_Interpreter.h"


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
