#include <stdio.h>
#include <stdlib.h>
#include "linear_algebra.h"

main()
{
	float matrix[LA_TAM][LA_TAM];
	int rows, cols;
	int i,j;
	
	la_read_one_pointer_matrix_s(matrix, &rows, &cols);
	puts("Inverted:");
	for (i=0; i < rows; i++, putchar('\n'))
		for (j=cols-1; j >= 0; j--)
			printf("%lf ", matrix[i][j]);
}
