#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int
//main(int argc, char *argv[], char *environ[])
main(int argc, char *argv[])
{
	void	*h;
	int	(*f) (int);
	
	h = dlopen("./hello.so", RTLD_LAZY);
	if (h == NULL)
	{
		fprintf (stderr, "%s", dlerror() );
		exit (EXIT_FAILURE);
	}

	f = dlsym (h, "hello");

	if (f == NULL)
	{
		fprintf (stderr, "%s\n", dlerror() );
		exit (EXIT_FAILURE);
	}

	printf ("%d\n", (f) (200));
	dlclose (h);

	exit (EXIT_SUCCESS);
}
