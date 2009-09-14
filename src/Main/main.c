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



//========================================
//Main
//========================================
int
main(
    int     argc,
    char**  argv
)
{
    int endCode = 0;
    Interpreter interpreter = NULL;
    
    //Logger_set(LOG_LEVEL_DEBUG, "Loader_parse");
    //Logger_set(LOG_LEVEL_TRACE, NULL);

    
    if (argv[1] == NULL)
    {
        printf("Input cria script file path.\n");
        goto END;
    }
    
    setlocale(LC_CTYPE, "");    //for wide charactors.
    
    interpreter = Interpreter_new();
    if (interpreter == NULL)
    {
        printf("Cria interpreter create error.\n");
        goto END;
    }
    
    
    Interpreter_compile(interpreter, String_mbsrtowcs(argv[1]));
    
    
    Interpreter_run(interpreter);
    
    
    //TODO: I have to improve that this function called, when the process is killed.
    //      Do I have to use a signal?
    Interpreter_dispose(interpreter);
    
    
    endCode = 1;
    
END:
    interpreter = NULL;
    
    return endCode;
}
