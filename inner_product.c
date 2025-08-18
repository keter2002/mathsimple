/*
    Calculates the inner product, norm and distance between two
    vectors/matrices.
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
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "cblas.h"
#include "linear_algebra.h"

float cos_angle_sx_sy(n, sx, incx, sy, incy)
float sx[], sy[];
{
    float inner_product;
    float normx, normy;

    inner_product = cblas_sdot(n, sx, incx, sy, incy);
    normx = sqrt(cblas_sdot(n, sx, incx, sx, incx));
    normy = sqrt(cblas_sdot(n, sy, incy, sy, incy));
    return inner_product / (normx * normy);
}

float matrix_eq_inner_product(m, n, a, lda)
float a[];
{
    int i, j;
    float sum;

    for (sum=i=0; i < m; i++)
        for (j=0; j < n; j++)
            sum += a[i * lda + j] * a[i * lda + j];
    return sum;
}

float distance_matrices(m, k, a, lda, b, ldb)
float a[], b[];
{
    int i, j;
    
    for (i=0; i < m; i++)
        for (j=0; j < k; j++)
            a[i * lda + j] = a[i * lda + j] - b[i * ldb + j];
    return sqrt(matrix_eq_inner_product(m, k, a, lda));
}

float distance_vectors(n, sx, incx, sy, incy)
float sx[], sy[];
{
    float *sz, dist;

    sz = malloc(sizeof(float) * n);
    cblas_scopy(n, sx, incx, sz, 1);
    cblas_saxpy(n, -1.f, sy, incy, sz, 1);
    dist = sqrt(cblas_sdot(n, sz, 1, sz, 1));
    free(sz);
    return dist;
}

float matrix_inner_product(m, n, k, a, lda, b, ldb)
float a[], b[];
{
    int i;
    float c[LA_SIZE * LA_SIZE], tr;

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, m, n, k, 1.f, a, lda, b, ldb, 1.f, c, LA_SIZE);
    for (tr=i=0; i < n; i++)
        tr += c[i * LA_SIZE + i];
    return tr;
}

main()
{
    float a[LA_SIZE*LA_SIZE], b[LA_SIZE*LA_SIZE];
    float inner_product, distance;
    int arows, acols;
    int brows, bcols;

    brows = arows = 0;
    la_read_matrices_s(a, &arows, &acols);
    la_read_matrices_s(b, &brows, &bcols);
    puts(arows < 2 && brows < 2? "Vectors:" : "Matrices:");
    la_show_matrix_s(a, arows, acols, LA_SIZE);
    la_show_matrix_s(b, brows, bcols, LA_SIZE);
    if (arows < 2 && brows < 2) {
        inner_product = cblas_sdot(acols, a, 1, brows? b : a, 1);
        distance = distance_vectors(acols, a, 1, brows? b : a, 1);
        if (brows)
            printf("Cosine of the angle between the vectors: %f\n",
                    cos_angle_sx_sy(acols, a, 1, b, 1));
    } else if (brows) {
        inner_product = matrix_inner_product(arows, bcols, acols, a, LA_SIZE, brows? b : a, LA_SIZE);
        distance = distance_matrices(arows, acols, a, LA_SIZE, b, LA_SIZE);
    } else {
        inner_product = matrix_eq_inner_product(arows, acols, a, LA_SIZE);
        distance = 0;
        printf("Other inner product: %f\n", matrix_inner_product(arows, acols, acols, a, LA_SIZE, a, LA_SIZE));
    }
    printf("Inner product: %f\n", inner_product);
    printf("Norm: %f\n", sqrt(inner_product));
    printf("Distance: %f\n", distance);
}
