#include "expression.c"
#include <stdio.h>

main(argc, argv)
char *argv[];
{
	dynamic_array fullexp;
	int i,next;
	double sum;

	if (argc < 2) {
		fputs("Usage: series_convergence expression\n", stderr);
		return 2;
	}
	infix_posfix(&fullexp, argv[1]);
	show_expr(&fullexp);
	for (i=1,sum=0, next=1; i <= 10000000; i++) {
		sum += evaluate(&fullexp, (double)i);
		if (i==next) {
			printf("%.5f [%d]\n", sum, i);
			next *= 10;
		}
	}
}
