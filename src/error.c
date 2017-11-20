#include "error.h"

#include "utils.h"

void ErrorCommand(int r, char *type)
{
	fprintf(stderr, "ERROR %d %s\n", r, type);
}

void ErrorParse(int r, int c, bool *hasError)
{
	if(!(*hasError))
	{
		fprintf(stderr, "ERROR %d %d\n", r, c); 
		(*hasError) = true;
	}
}
