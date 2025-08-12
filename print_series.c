#include "expression.c"
#include <stdio.h>

main(argc, argv)
char *argv[];
{
	dynamic_array fullexp;
	int i, start, n;

	if (argc < 4) {
		fputs("Usage: print_series start n expression\n", stderr);
		return 2;
	}
	i = start = atoi(argv[1]);
	n = atoi(argv[2]);
	infix_posfix(&fullexp, argv[3]);
	show_expr(&fullexp);
	for (; i < start+n; i++)
		printf(i == start+n-1? "%.5f\n" : "%.5f ", evaluate(&fullexp, (double)i));
}
