/*
    linear_algebra.h - v2.1.0
    Declarations of utility functions to deal with matrices.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.1.0  la_arg_precision
        v2.0.0  stream args and lda in la_read_matrices_*
        v1.0.0  First version

    linear_algebra.h is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef LA_H
#define LA_H

#include <stdio.h>

#include "la_eps.h"

#define LA_SIZE 30

extern la_arg_precision;

/* WARNING: these functions do not handle invalid memory access. */

void la_show_matrix_s(FILE *stream, float *a, int rows, int cols, int lda);
void la_show_pointer_matrix_s(FILE *stream, float a[][LA_SIZE], int rows, int cols);

void la_show_matrix_d(FILE *stream, double *a, int rows, int cols, int lda);
void la_show_pointer_matrix_d(FILE *stream, double a[][LA_SIZE], int rows, int cols);

void la_read_matrices_s(FILE *stream, float *a, int *rows, int *cols, int lda);
void la_read_matrices_d(FILE *stream, double *a, int *rows, int *cols, int lda);

/* Have checks for maximum column length, but not for row length.*/

void la_read_one_pointer_matrix_s(FILE *stream, float a[][LA_SIZE], int *rows, int *cols);
void la_read_one_pointer_matrix_d(FILE *stream, double a[][LA_SIZE], int *rows, int *cols);

#endif
