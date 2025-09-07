/*
    linear_solver - v1.0.0
    Solves a linear system of equations using Gauss-Jordan elimination.
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
#include "linear_algebra.h"

main(argc)
{
    double matrix[LA_SIZE][LA_SIZE];
    int rows, cols;
    int i,j,k;
    
    if (argc >= 2) {
        fputs("Usage: linear_solver\n"
              "Solves a linear system of equations using Gauss-Jordan elimination.\n\n"
              "The system is read from stdin in general form, each cell is the coefficient of\n"
              "a variable, the coefficients should be in the same order for each equation and\n"
              "the right side of the equations should stay in the last column.\n"
              "For example, a system with the equations: x + 3/2*y = 3 and 8*x + 18*y = 30 is\n"
              "read as below:\n"
              "1 1.5 3\n"
              "8 18 30\n", stderr);
        return argc > 2? 2 : 0;
    }
    la_read_one_pointer_matrix_d(matrix, &rows, &cols);
    puts("Linear System:");
    la_show_pointer_matrix_d(matrix, rows, cols);
    for (k=0; k < rows; k++) {
        for (i=k+1; i < cols; i++)
            matrix[k][i] /= matrix[k][k];
        matrix[k][k] = 1.f;
        for (i=k+1; i < rows; i++) {
            for (j=k+1; j < cols; j++)
                matrix[i][j] -= matrix[k][j]*matrix[i][k];
            matrix[i][k] = 0;
        }
        printf("Step %0d:\n", k+1);
        la_show_pointer_matrix_d(matrix, rows, cols);
    }
    if (rows+1==cols)
        for (k=rows-2; k >= 0; k--)
            for (i=k; i < rows-1; i++) {
                matrix[k][cols-1] -= matrix[i+1][cols-1]*matrix[k][i+1];
                matrix[k][i+1] = 0;
            }
    else
        for (k=rows-1; k > 0; k--)    
            for (i=k-1; i >= 0; i--) {
                for (j=k+1; j < cols; j++)
                    matrix[i][j] -= matrix[i][k]*matrix[k][j];
                matrix[i][k] = 0;
            }
    puts("Solution");
    la_show_pointer_matrix_d(matrix, rows, cols);
}
