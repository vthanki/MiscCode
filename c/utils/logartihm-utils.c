#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	double op;

	if (!strcmp(argv[1], "-l")) {
		/* log */
		sscanf(argv[2], "%lf", &op);
		printf("%lf\n", log10(op));
	} else if (!strcmp(argv[1], "-a")) {
		/* antilog */
		sscanf(argv[2], "%lf", &op);
		printf("%lf\n", pow(10, op));
	} else if (!strcmp(argv[1], "-s")) {
		/* logarithmic scale */
		double start, end;
		double startl, endl, interval, delta;
		int i;
		sscanf(argv[2], "%lf", &start);
		sscanf(argv[3], "%lf", &end);
		sscanf(argv[4], "%lf", &interval);

		startl = log10(start);
		endl= log10(end);

		delta = (endl - startl) / (interval-1);
		for (i = 0; i < (int) interval; i++) {
			printf("%lf\n", pow(10, startl));
			startl += delta;
		}
	}

	return 0;
}
