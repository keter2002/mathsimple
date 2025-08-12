#include <stdio.h>
#include <stdlib.h>

main(argc, argv)
char *argv[];
{
	int num, factor;

	if (argc < 2) {
		puts("Usage: ./factorizer x");
		return 2;
	}
	num = atoi(argv[1]);
	factor = 2;
	puts("1");
	while (num != 1)
		if (!(num % factor)) {
			printf("%d\n", factor);
			num /= factor;
			factor = 2;
		} else
			factor++;
}
