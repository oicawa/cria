#include <stdio.h>

#include "Interpreter.h"
#include "Logger.h"



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
    
    
    interpreter = Interpreter_new();
    if (interpreter == NULL)
    {
        printf("Cria interpreter create error.\n");
        goto END;
    }
    
    
    Interpreter_compile(interpreter, argv[1]);
    
    
    Interpreter_run(interpreter);
    
    
    //TODO: I have to improve that this function called, when the process is killed.
    //      Use signal?
    Interpreter_dispose(interpreter);
    
    
    endCode = 1;
    
END:
    interpreter = NULL;
    
    return endCode;
}
