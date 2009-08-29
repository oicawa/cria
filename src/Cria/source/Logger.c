/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
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




