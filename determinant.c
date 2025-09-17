/*
    determinant - v1.0.1
    Find the determinant of a matrix.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    determinant is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>
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

main(argc)
{
    double matrix[LA_SIZE][LA_SIZE];
    int rows, cols;

    if (argc >= 2) {
        fputs("Usage: determinant\n"
              "Find the determinant of a matrix.\n\n"
              "The matrix is read from stdin, in the format:\n"
              "1 2 3 ...\n"
              "4 5 6 ...\n"
              ". . . ...\n"
              ". . . ...\n"
              ". . . ...\n", stderr);
        return argc > 2? 2 : 0;
    }
    la_read_one_pointer_matrix_d(stdin, matrix, &rows, &cols);
    printf("%lf\n", determinant(matrix, rows, cols));
}
