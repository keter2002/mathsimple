/*
    matmul - v1.0.1
    Do matrix multiplication.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    matmul is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <cblas.h>
#include "linear_algebra.h"

main()
{
    double a[LA_SIZE*LA_SIZE], b[LA_SIZE*LA_SIZE], c[LA_SIZE*LA_SIZE];
    int rows_a, cols_a, rows_b, cols_b;

    la_read_matrices_d(stdin, a, &rows_a, &cols_a, LA_SIZE);
    la_read_matrices_d(stdin, b, &rows_b, &cols_b, LA_SIZE);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows_a, cols_b,
                cols_a, 1.f, a, LA_SIZE, b, LA_SIZE, 1.f, c, LA_SIZE);
    la_show_matrix_d(stdout, c, rows_a, cols_b, LA_SIZE);
}
