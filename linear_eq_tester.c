/*
    Tests if a solution to a linear system is correct.
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
#include "linear_algebra.h"

double matrix[LA_SIZE][LA_SIZE];
double values[LA_SIZE];

main()
{
    int rows, cols;
    int i,j,k;
    double sum;

    la_read_one_pointer_matrix_d(matrix, &rows, &cols);
    puts("Linear System:");
    la_show_pointer_matrix_d(matrix, rows, cols);
    for (i=0; i < cols-1; i++) {
        printf("Variable %02d: ", i+1);
        scanf("%lf", &values[i]);
    }
    for (k=0; k < rows; k++) {
        for (sum=j=0; j < cols-1; j++)
            sum += matrix[k][j]*values[j];
        printf("%lf ", sum);
        puts(fabs(fabs(sum)-fabs(matrix[k][cols-1])) < LA_EPS? "Equal" : "Not equal");
    }
}
