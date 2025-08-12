#include <stdio.h>
#include <stdlib.h>
#include <math.h>

main(argc, argv)
char *argv[];
{
	float a, b, c;
	float delta;

	if (argc < 4) {
		puts("Usage: ./bhaskara a b c");
		return 2;
	}
	a = atof(argv[1]);
	b = -atof(argv[2]);
	c = atof(argv[3]);
	if ((delta = b*b - 4*a*c) < 0) {
		puts("No real solutions.");
		return 0;
	}
	a *= 2;
	printf("D:%f 2A:%f -B:%f C:%f\n", delta, a, b, c);
	printf("X1:%f X2:%f\n", (b + sqrt(delta)) / a, (b - sqrt(delta)) / a);
}
