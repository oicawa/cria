#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include "Cria.h"
#include "Logger.h"

#define LOG_FILE_PATH   "./cria.log"

static int      static_logLevel = LOG_LEVEL_INFORMATION;
static char*    static_target = NULL;
static char*    static_base_format = "%c[%32s(% 5d): %-32s] ";



void
Logger_set(
    int         logLevel,
    char*       target
)
{
    static_logLevel = logLevel;
    static_target = target;
}



void
Logger_write(
    char*       fileName,
    const char* funcName,
    int         line,
    int         logLevel,
    char*       format,
    ...
)
{
    FILE*   file = NULL;
    
    
    if (static_logLevel < logLevel)
        return;

    
    if (static_target != NULL)
    {
        char* target = strstr(static_target, funcName);
        if (target == NULL)
            return;
    }
    
    
    char level;
    if (logLevel == LOG_LEVEL_ERROR)
        level = 'E';
    else if (logLevel == LOG_LEVEL_WARNING)
        level = 'W';
    else if (logLevel == LOG_LEVEL_INFORMATION)
        level = 'I';
    else if (logLevel == LOG_LEVEL_TRACE)
        level = 'T';
    else if (logLevel == LOG_LEVEL_DEBUG)
        level = 'D';
    else if (logLevel == LOG_LEVEL_CORE)
        level = 'C';
    else
        level = '?';
    
    va_list argp;
    va_start(argp, format);

    if ((file = fopen(LOG_FILE_PATH, "a")) == NULL)
    {
        fprintf(stderr, "Log file open error. (./cria.log)\n");
        return;
    }
    
    fprintf(file, static_base_format, level, fileName, line, funcName);
    vfprintf(file, format, argp );
    fprintf(file, "\n");
    
    fclose(file);
}




