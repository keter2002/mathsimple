#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <stdio.h>

#include "la_eps.h"

#define LA_TAM 30

void la_show_matrix_s(float *a, int rows, int cols, int lda);
void la_show_pointer_matrix_s(float a[][LA_TAM], int rows, int cols);

void la_show_matrix_d(double *a, int rows, int cols, int lda);
void la_show_pointer_matrix_d(double a[][LA_TAM], int rows, int cols);

void la_read_matrices_s(float *a, int *rows, int *cols);
void la_read_matrices_d(double *a, int *rows, int *cols);
void la_read_one_pointer_matrix_s(float a[][LA_TAM], int *rows, int *cols);
void la_read_one_pointer_matrix_d(double a[][LA_TAM], int *rows, int *cols);

#include "la_print_know_constant.h"

#endif
