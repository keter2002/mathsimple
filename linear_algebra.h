#ifndef H_LA
#define H_LA

#include <stdio.h>

#include "eps_la.h"

#define SIZE_LA 30

void show_matrix_s_la(float *a, int rows, int cols, int lda);
void show_pointer_matrix_s_la(float a[][SIZE_LA], int rows, int cols);

void show_matrix_d_la(double *a, int rows, int cols, int lda);
void show_pointer_matrix_d_la(double a[][SIZE_LA], int rows, int cols);

void read_matrices_s_la(float *a, int *rows, int *cols);
void read_matrices_d_la(double *a, int *rows, int *cols);
void read_one_pointer_matrix_s_la(float a[][SIZE_LA], int *rows, int *cols);
void read_one_pointer_matrix_d_la(double a[][SIZE_LA], int *rows, int *cols);

#include "print_know_constant_la.h"

#endif
