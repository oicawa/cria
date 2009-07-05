#include <stdio.h>
#include <memory.h>

#include "Interpreter.h"

#define BUFFER_SIZE 80

//========================================
//Main
//========================================
int
main(
    int argc,
    char** argv
)
{
    int endCode = 0;
    Interpreter interpreter = NULL;
    FILE* file = NULL;
    Boolean is_stdout = TRUE;
    char buffer[BUFFER_SIZE + 1]; 
    char* lf = NULL;
    int line = 1;
    String file_path = NULL;
    
    if (argv[1] == NULL)
    {
        file = stdin;
        file_path = String_new("<<stdin>>");
    }
    else
    {
        file = fopen(argv[1], "r");
        file_path = String_new(argv[1]);
        is_stdout = FALSE;
    }
    
    if (file == NULL)
    {
        fprintf(stderr, "File open error. ('%s')\n", file_path);
        goto END;
    }
    
    
    interpreter = Interpreter_new();
    if (interpreter == NULL)
    {
        printf("Cria interpreter create error.\n");
        goto END;
    }
    
    while (feof(file) == 0)
    {
        memset(buffer, 0x00, sizeof(buffer));
        lf = NULL;
        
        fgets(buffer, sizeof(buffer) - sizeof(char), file);
        lf = strchr(buffer, '\n'); 
        if (lf != NULL)
        {
            *lf = '\0';
        }
        else if (feof(file) == FALSE)
        {
            fprintf(stderr, "[%s] line %3d : Too long sentence. Please get the line under %d columns.\n\n", argv[1], line, BUFFER_SIZE);
            goto END;
        }
        
        //TODO: I have to implement logic for that 1 line is over buffer size.
        Interpreter_run(interpreter, file_path, line, buffer);
        
        line++;
    }
    
	
    Interpreter_dispose(interpreter);
    
    
    endCode = 1;
    
END:
    interpreter = NULL;
    if (is_stdout == TRUE)
        fclose(file);
    
    return endCode;
}
