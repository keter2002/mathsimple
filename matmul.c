#include <stdio.h>
#include <cblas.h>
#include "linear_algebra.h"

main()
{
	double a[LA_TAM*LA_TAM], b[LA_TAM*LA_TAM], c[LA_TAM*LA_TAM];
	int rows_a, cols_a, rows_b, cols_b;

	la_read_matrices_d(a, &rows_a, &cols_a);
	la_read_matrices_d(b, &rows_b, &cols_b);
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows_a, cols_b,
			cols_a, 1.f, a, LA_TAM, b, LA_TAM, 1.f, c, LA_TAM);
	la_show_matrix_d(c, rows_a, cols_b, LA_TAM);
}
