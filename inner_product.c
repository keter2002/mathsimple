/*
    inner_product - v2.1.0
    Calculates the inner product, norm and distance between two
    vectors/matrices. If the input is two vectors it calculates the cosine of
    the angle beetwen them too.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.1.0  Doubly precision
        v2.0.0  Printing precision argument and remove last space in output
        v1.0.1  stream and lda arguments in la_read_matrics_s and la_show_matrix_s
        v1.0.0  First version

    inner_product is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>

#include <getopt.h>

#include "cblas.h"

#include "linear_algebra.h"


double cos_angle_sx_sy(n, sx, incx, sy, incy)
double sx[], sy[];
{
    double inner_product;
    double normx, normy;

    inner_product = cblas_ddot(n, sx, incx, sy, incy);
    normx = sqrt(cblas_ddot(n, sx, incx, sx, incx));
    normy = sqrt(cblas_ddot(n, sy, incy, sy, incy));
    return inner_product / (normx * normy);
}

double matrix_eq_inner_product(m, n, a, lda)
double a[];
{
    int i, j;
    double sum;

    for (sum=i=0; i < m; i++)
        for (j=0; j < n; j++)
            sum += a[i * lda + j] * a[i * lda + j];
    return sum;
}

double distance_matrices(m, k, a, lda, b, ldb)
double a[], b[];
{
    int i, j;
    
    for (i=0; i < m; i++)
        for (j=0; j < k; j++)
            a[i * lda + j] = a[i * lda + j] - b[i * ldb + j];
    return sqrt(matrix_eq_inner_product(m, k, a, lda));
}

double distance_vectors(n, sx, incx, sy, incy)
double sx[], sy[];
{
    double *sz, dist;

    sz = malloc(sizeof(double) * n);
    cblas_dcopy(n, sx, incx, sz, 1);
    cblas_daxpy(n, -1.f, sy, incy, sz, 1);
    dist = sqrt(cblas_ddot(n, sz, 1, sz, 1));
    free(sz);
    return dist;
}

double matrix_inner_product(m, n, k, a, lda, b, ldb)
double a[], b[];
{
    int i;
    double c[LA_SIZE * LA_SIZE], tr;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, m, n, k, 1.f, a, lda, b, ldb, 1.f, c, LA_SIZE);
    for (tr=i=0; i < n; i++)
        tr += c[i * LA_SIZE + i];
    return tr;
}

main(argc, argv)
char *argv[];
{
    double a[LA_SIZE*LA_SIZE], b[LA_SIZE*LA_SIZE];
    double inner_product, distance;
    int arows, acols;
    int brows, bcols;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    int opt;

    for (; (opt = getopt_long(argc, argv, "p:", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            la_arg_precision = atoi(optarg);
            break;
        case '?':
            fputs("Try 'inner_product --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: inner_product [OPTION]\n"
                  "Calculates the inner product, norm and distance between two\n"
                  "vectors/matrices. If the input is two vectors it calculates the cosine of\n"
                  "the angle beetwen them too.\n\n"
                  "The matrices/vectors are read from standard input.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n",
                  stdout);
            return 0;
        }

    brows = arows = 0;
    la_read_matrices_d(stdin, a, &arows, &acols, LA_SIZE);
    la_read_matrices_d(stdin, b, &brows, &bcols, LA_SIZE);
    puts(arows < 2 && brows < 2? "Vectors:" : "Matrices:");
    la_show_matrix_d(stdout, a, arows, acols, LA_SIZE);
    la_show_matrix_d(stdout, b, brows, bcols, LA_SIZE);
    if (arows < 2 && brows < 2) {
        inner_product = cblas_ddot(acols, a, 1, brows? b : a, 1);
        distance = distance_vectors(acols, a, 1, brows? b : a, 1);
        if (brows)
            printf("Cosine of the angle between the vectors: %.*lf\n",
                    la_arg_precision, cos_angle_sx_sy(acols, a, 1, b, 1));
    } else if (brows) {
        inner_product = matrix_inner_product(arows, bcols, acols, a, LA_SIZE, brows? b : a, LA_SIZE);
        distance = distance_matrices(arows, acols, a, LA_SIZE, b, LA_SIZE);
    } else {
        inner_product = matrix_eq_inner_product(arows, acols, a, LA_SIZE);
        distance = 0;
        printf("Other inner product: %.*lf\n", la_arg_precision,
               matrix_inner_product(arows, acols, acols, a, LA_SIZE, a, LA_SIZE));
    }
    printf("Inner product: %.*lf\n", la_arg_precision, inner_product);
    printf("Norm: %.*lf\n", la_arg_precision, sqrt(inner_product));
    printf("Distance: %.*lf\n", la_arg_precision, distance);
}
