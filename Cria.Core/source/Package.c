#include <stdio.h>

int
__Package_load__(int i)
{
	printf ("Hello, %d\n", i);
	return (i + 1);
}
