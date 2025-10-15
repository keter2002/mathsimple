/*
    determinant - v1.1.0
    Find the determinant of a matrix.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.1.0  Printing precision argument
        v1.0.1  stream argument in la_read_one_pointer_matrix_d
        v1.0.0  First version

    determinant is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "linear_algebra.h"


double determinant(m, rows, cols)
double m[][LA_SIZE];
{
    int p[LA_SIZE+1];
    int k_indexes[LA_SIZE];
    int i, j, k;

    double det = 0, prod;
    int sig = -1;

    /* Do first iteration of the summation */
    for (prod=m[0][0], k=1; k < cols; k++)
        prod *= m[k][k];
    det += prod;
    
    /* Regressive QuickPerm */
    for (i=0; i < cols; i++) {
        k_indexes[i] = i;
        p[i] = i;
    }
    p[i] = i;
    i=1;
    while (i < cols) {
        p[i]--;
        j = i%2? p[i] : 0;
        
        /* Swap */
        k_indexes[i] ^= k_indexes[j];
        k_indexes[j] ^= k_indexes[i];
        k_indexes[i] ^= k_indexes[j];
        
        /* Do one iteration of the summation */
        for (prod=m[0][k_indexes[0]], k=1; k < cols; k++)
            prod *= m[k][k_indexes[k]];
        det += sig*prod;
        sig *= -1;

        i = 1;
        while (!p[i]) {
            p[i] = i;
            i++;
        }
    }
    return det;
}

main(argc, argv)
char *argv[];
{
    double matrix[LA_SIZE][LA_SIZE];
    int rows, cols;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    int opt;
    int arg_precision = 6;

    for (; (opt = getopt_long(argc, argv, "p:", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case '?':
            fputs("Try 'determinant --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: determinant [OPTION]\n"
                  "Find the determinant of a matrix.\n\n"
                  "The matrix is read from standard input, in the format:\n"
                  "1 2 3 ...\n"
                  "4 5 6 ...\n"
                  ". . . ...\n"
                  ". . . ...\n"
                  ". . . ...\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n"
                  , stdout);
            return 0;
        }

    la_read_one_pointer_matrix_d(stdin, matrix, &rows, &cols);
    printf("%.*lf\n", arg_precision, determinant(matrix, rows, cols));
}
