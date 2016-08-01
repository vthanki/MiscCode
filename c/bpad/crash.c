#include <stdio.h>
#include "src/client/linux/handler/exception_handler.h"

void crash()
{
	int *x = NULL;
	*x = 100;
}

int main()
{
	crash();
	return 0;
}
