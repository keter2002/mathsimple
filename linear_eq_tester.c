/*
    linear_eq_tester - v2.0.0
    Tests if a solution to a linear system is correct.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.0  Printing precision argument and remove last space in output
        v1.0.1  stream arg in la_read_one_pointer_matrix_d() and
                la_show_pointer_matrix_d()
        v1.0.0  First version

    linear_eq_tester is free software: you can redistribute it and/or modify it
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

#include "linear_algebra.h"


double matrix[LA_SIZE][LA_SIZE];
double values[LA_SIZE];

main(argc, argv)
char *argv[];
{
    int rows, cols;
    int i,j,k;
    double sum;

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
            fputs("Try 'linear_eq_tester --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: linear_eq_tester [OPTION]\n"
                  "Tests if a solution to a linear system is correct.\n\n"
                  "The system of equations is read from standard input in a matrix. The program\n"
                  "will ask for the solution in the same order that the coefficients appear in the\n"
                  "system\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n",
                  stdout);
            return 0;
        }

    la_read_one_pointer_matrix_d(stdin, matrix, &rows, &cols);
    puts("Linear System:");
    la_show_pointer_matrix_d(stdout, matrix, rows, cols);
    for (i=0; i < cols-1; i++) {
        printf("Variable %02d: ", i+1);
        scanf("%lf", &values[i]);
    }
    for (k=0; k < rows; k++) {
        for (sum=j=0; j < cols-1; j++)
            sum += matrix[k][j]*values[j];
        printf("%.*lf ", la_arg_precision, sum);
        puts(fabs(fabs(sum)-fabs(matrix[k][cols-1])) < LA_EPS? "Equal" : "Not equal");
    }
}
