#ifndef H_LA
#define H_LA

#include <stdio.h>

#include "la_eps.h"

#define SIZE_LA 30

void la_show_matrix_s(float *a, int rows, int cols, int lda);
void la_show_pointer_matrix_s(float a[][SIZE_LA], int rows, int cols);

void la_show_matrix_d(double *a, int rows, int cols, int lda);
void la_show_pointer_matrix_d(double a[][SIZE_LA], int rows, int cols);

void la_read_matrices_s(float *a, int *rows, int *cols);
void la_read_matrices_d(double *a, int *rows, int *cols);
void la_read_one_pointer_matrix_s(float a[][SIZE_LA], int *rows, int *cols);
void la_read_one_pointer_matrix_d(double a[][SIZE_LA], int *rows, int *cols);

#include "la_print_know_constant.h"

#endif
