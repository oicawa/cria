#include <stdio.h>
#include <stdlib.h>
#include "Memory.h"

int
main(void)
{
	char c = ' ';
	void* pointer = NULL;
	
	while(c != 'q')
	{
		scanf("%c", &c);
		pointer = Memory_malloc(1000000);
		//pointer = malloc(1000000);
	}
	
	printf("Quit!\n");

    return 1;
}

