/*
    Do matrix multiplication.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <cblas.h>
#include "linear_algebra.h"

main()
{
    double a[SIZE_LA*SIZE_LA], b[SIZE_LA*SIZE_LA], c[SIZE_LA*SIZE_LA];
    int rows_a, cols_a, rows_b, cols_b;

    la_read_matrices_d(a, &rows_a, &cols_a);
    la_read_matrices_d(b, &rows_b, &cols_b);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows_a, cols_b,
                cols_a, 1.f, a, SIZE_LA, b, SIZE_LA, 1.f, c, SIZE_LA);
    la_show_matrix_d(c, rows_a, cols_b, SIZE_LA);
}
