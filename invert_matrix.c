/*
    invert_matrix - v1.1.0
    Inverts a matrix.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.1.0  Printing precision argument
        v1.0.1  stream argument in la_read_one_pointer_matrix_s
        v1.0.0  First version

    invert_matrix is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "linear_algebra.h"


main(argc, argv)
char *argv[];
{
    double matrix[LA_SIZE][LA_SIZE];
    int rows, cols;
    int i,j;

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
            fputs("Try 'invert_matrix --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: invert_matrix [OPTION]\n"
                  "Inverts a matrix.\n\n"
                  "The matrix is read from standard input.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n",
                  stdout);
            return 0;
        }

    la_read_one_pointer_matrix_d(stdin, matrix, &rows, &cols);
    puts("Inverted:");
    for (i=0; i < rows; i++, putchar('\n'))
        for (j=cols-1; j >= 0; j--)
            printf("%.*lf ", arg_precision, matrix[i][j]);

    return 0;
}
