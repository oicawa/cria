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
    List args,
    CriaBlock block
);



CriaId
CriaIO_read(
    Interpreter interpreter,
    CriaId object,
    List args,
    CriaBlock block
);



#endif
