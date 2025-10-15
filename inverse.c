/*
    inverse - v2.0.0
    Finds the inverse of a given matrix.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.0  Printing precision argument and remove last space in output
        v1.0.1  Use la_read_matrices_d to read input matrix and pass stream
                argument in la_show_matrix_d
        v1.0.0  First version

    inverse is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <getopt.h>

#include "linear_algebra.h"


#define LDA (LA_SIZE<<1)

double matrix[LA_SIZE*LDA];

main(argc, argv)
char *argv[];
{
    int rows, cols;
    int i,j,k;

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
            fputs("Try 'inverse --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: inverse [OPTION]\n"
                  "Finds the inverse of a given matrix.\n\n"
                  "The matrix is read from standard input.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n",
                  stdout);
            return 0;
        }

    la_read_matrices_d(stdin, matrix, &rows, &cols, LDA);
    /* Concatenate with inverse: */
    for (i=0; i < rows; i++)
        for (j=0; j < cols; j++)
            matrix[LDA * i + j + cols] = i==j;
    puts("M and I:");
    for (i=0; i < rows; i++, putchar('\n'))
        for (j=0; j < cols<<1; j++)
            printf("%.*f ", la_arg_precision, matrix[LDA * i + j]);
    for (k=0; k < rows; k++) {
        /* Divide the row akj by akk. */
        for (i=k+1; i < cols<<1; i++)
            matrix[LDA * k + i] /= matrix[LDA * k + k];
        /* Ensures akk equals to one. */
        matrix[LDA * k + k] = 1.f;
        /* Subtract the next rows a(i=k+1)j by aik times akj. */
        for (i=k+1; i < rows; i++) {
            for (j=k+1; j < cols<<1; j++)
                matrix[LDA * i + j] -= matrix[LDA * i + k] * matrix[LDA * k + j];
            /* Ensures that aik equals to zero. */
            matrix[LDA * i + k] = 0;
        }
        printf("Step %0d:\n", k+1);
        la_show_matrix_d(stdout, matrix, rows, cols, LDA);
    }
    /* From the bottom up, subtract row akj times aik. */
    for (k=rows-1; k > 0; k--)    
        for (i=k-1; i >= 0; i--) {
            for (j=k+1; j < cols<<1; j++)
                matrix[LDA * i + j] -= matrix[LDA * k + j] * matrix[LDA * i + k];
            /* Guarantees aik equals to zero. */
            matrix[LDA * i + k] = 0;
        }
    puts("M:");
    la_show_matrix_d(stdout, matrix, rows, cols, LDA);
    puts("M^-1:");
    la_show_matrix_d(stdout, &matrix[cols], rows, cols, LDA);
}
