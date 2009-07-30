#ifndef PUBLIC_CRIAIO_H_INCLUDED
#define PUBLIC_CRIAIO_H_INCLUDED


#include "Cria.h"
#include "CriaId.h"
#include "List.h"
#include "Interpreter.h"


CriaId
CriaIO_write(
    Interpreter interpreter,
    CriaId object,
    List        args
);



CriaId
CriaIO_read(
    Interpreter interpreter,
    CriaId object,
    List        args
);



#endif
