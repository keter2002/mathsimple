/*
    Definitions of utility functions to deal with matrices.
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

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "linear_algebra.h"

void la_show_matrix_s(a, rows, cols, lda)
float *a;
{
    int i, j;
    float aij;

    for (i=0; i < rows; i++, putchar('\n'))
        for (j=0; j < cols; j++) {
            aij = a[i * lda + j];
                LA_PRINT_KNOW_CONSTANT(aij)
            else
                printf("%f ", aij);
        }
}

void la_show_pointer_matrix_s(a, rows, cols)
float a[][LA_SIZE];
{
    int i;
    
    for (i=0; i < rows; i++)
        la_show_matrix_s(a[i], 1, cols, LA_SIZE);
}

void la_show_matrix_d(a, rows, cols, lda)
double *a;
{
    int i, j;
    double aij;

    for (i=0; i < rows; i++, putchar('\n'))
        for (j=0; j < cols; j++) {
            aij = a[i * lda + j];
                LA_PRINT_KNOW_CONSTANT(aij)
            else
                printf("%lf ", aij);
        }
}

void la_show_pointer_matrix_d(a, rows, cols)
double a[][LA_SIZE];
{
    int i;
    
    for (i=0; i < rows; i++)
        la_show_matrix_d(a[i], 1, cols, LA_SIZE);
}

extern double torfnum_atof();

void la_read_one_pointer_matrix_s(a, rows, cols)
float a[][LA_SIZE];
int *rows, *cols;
{
    char *line, *p;
    size_t size;

    *rows = size = 0;
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " ")) {
            assert(*cols < LA_SIZE);
            a[*rows][(*cols)++] = torfnum_atof(p);
        }
        ++*rows;
    }
    free(line);
}

void la_read_one_pointer_matrix_d(a, rows, cols)
double a[][LA_SIZE];
int *rows, *cols;
{
    char *line, *p;
    size_t size;

    *rows = size = 0;
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " ")) {
            assert(*cols < LA_SIZE);
            a[*rows][(*cols)++] = torfnum_atof(p);
        }
        ++*rows;
    }
    free(line);
}

void la_read_matrices_s(a, rows, cols)
float *a;
int *rows, *cols;
{
    char *line, *p;
    size_t size;
    
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows * LA_SIZE + (*cols)++] = torfnum_atof(p);
        ++*rows;
    }
    free(line);
}

void la_read_matrices_d(a, rows, cols)
double *a;
int *rows, *cols;
{
    char *line, *p;
    size_t size;
    
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows * LA_SIZE + (*cols)++] = torfnum_atof(p);
        ++*rows;
    }
    free(line);
}
