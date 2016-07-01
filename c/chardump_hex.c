#include <stdio.h>
#include <ctype.h>

int main()
{
	char x[16];
	int i;

	for (i = 0; i < sizeof(x); i++) {
		scanf("%x", &x[i]);
	}

	for (i = 0; i < sizeof(x); i++) {
		printf("%c", isprint(x[i]) ? x[i] : '.');
	}
	printf("\n");
	return 0;
}

