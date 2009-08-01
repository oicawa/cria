#ifndef PUBLIC_LOGGER_H_INCLUDED
#define PUBLIC_LOGGER_H_INCLUDED


#include "Cria.h"




void
Logger_set(
    int         logLevel,
    char*       target
);



void
Logger_write(
    char*       fileName,
    const char* funcName,
    int         line,
    int         logLevel,
    char*       format,
    ...
);
#define Logger_err(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_ERROR, __VA_ARGS__))
#define Logger_wrn(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_WARNING, __VA_ARGS__))
#define Logger_inf(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_INFORMATION, __VA_ARGS__))
#define Logger_trc(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_TRACE, __VA_ARGS__))
#define Logger_dbg(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_DEBUG, __VA_ARGS__))
#define Logger_cor(...)\
    (Logger_write(__FILE__, __func__, __LINE__, LOG_LEVEL_CORE, __VA_ARGS__))



#endif  /* PUBLIC_LOGGER_H_INCLUDED */
