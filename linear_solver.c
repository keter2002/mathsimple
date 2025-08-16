/*
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

main()
{
    double matrix[SIZE_LA][SIZE_LA];
    int rows, cols;
    int i,j,k;

    read_one_pointer_matrix_d_la(matrix, &rows, &cols);
    puts("Linear System:");
    show_pointer_matrix_d_la(matrix, rows, cols);
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
        show_pointer_matrix_d_la(matrix, rows, cols);
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
    show_pointer_matrix_d_la(matrix, rows, cols);
}
