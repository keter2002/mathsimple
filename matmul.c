/*
    matmul - v2.0.0
    Do matrix multiplication.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.0  Printing precision argument and remove last space in output
        v1.0.1  stream and lda argument in la_read_matrices_d and stream arg in
                la_show_matrix_d
        v1.0.0  First version


    matmul is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include <cblas.h>

#include "linear_algebra.h"


main(argc, argv)
char *argv[];
{
    double a[LA_SIZE*LA_SIZE], b[LA_SIZE*LA_SIZE], c[LA_SIZE*LA_SIZE];
    int rows_a, cols_a, rows_b, cols_b;

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
            fputs("Try 'matmul --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: matmul [OPTION]\n"
                  "Do matrix multiplication.\n\n"
                  "The matrices are read from standard input.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n",
                  stdout);
            return 0;
        }

    la_read_matrices_d(stdin, a, &rows_a, &cols_a, LA_SIZE);
    la_read_matrices_d(stdin, b, &rows_b, &cols_b, LA_SIZE);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows_a, cols_b,
                cols_a, 1.f, a, LA_SIZE, b, LA_SIZE, 1.f, c, LA_SIZE);
    la_show_matrix_d(stdout, c, rows_a, cols_b, LA_SIZE);
}
