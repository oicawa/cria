#include <stdio.h>

#include "Interpreter.h"



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
    int result = 0;
    
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
    
    
    result = Interpreter_compile(interpreter, argv[1]);
    if (result == FALSE)
    {
        printf("Cria interpreter compile error.\n");
        goto END;
    }
    
    
    Interpreter_run(interpreter);
    
    
    endCode = 1;
    
END:
    interpreter = NULL;
    
    return endCode;
}
