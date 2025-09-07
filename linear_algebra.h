/*
    linear_algebra.h - v0.1.0
    Declarations of utility functions to deal with matrices.
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

#ifndef LA_H
#define LA_H

#include <stdio.h>

#include "la_eps.h"

#define LA_SIZE 30

/* WARNING: these functions do not handle invalid memory access. */

void la_show_matrix_s(float *a, int rows, int cols, int lda);
void la_show_pointer_matrix_s(float a[][LA_SIZE], int rows, int cols);

void la_show_matrix_d(double *a, int rows, int cols, int lda);
void la_show_pointer_matrix_d(double a[][LA_SIZE], int rows, int cols);

void la_read_matrices_s(float *a, int *rows, int *cols);
void la_read_matrices_d(double *a, int *rows, int *cols);

/* Have checks for maximum column length, but not for row length.*/

void la_read_one_pointer_matrix_s(float a[][LA_SIZE], int *rows, int *cols);
void la_read_one_pointer_matrix_d(double a[][LA_SIZE], int *rows, int *cols);

#include "la_print_know_constant.h"

#endif
