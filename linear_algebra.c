#include <string.h>
#include <stdlib.h>
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
float a[][LA_TAM];
{
	int i;

	for (i=0; i < rows; i++)
		la_show_matrix_s(a[i], 1, cols, LA_TAM);
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
double a[][LA_TAM];
{
	int i;

	for (i=0; i < rows; i++)
		la_show_matrix_d(a[i], 1, cols, LA_TAM);
}

void la_read_one_pointer_matrix_s(a, rows, cols)
float a[][LA_TAM];
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
		for (p=strtok(line, " "); p; p = strtok(NULL, " "))
			a[*rows][(*cols)++] = atof(p);
		++*rows;
	}
	free(line);
}

void la_read_one_pointer_matrix_d(a, rows, cols)
double a[][LA_TAM];
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
		for (p=strtok(line, " "); p; p = strtok(NULL, " "))
			a[*rows][(*cols)++] = atof(p);
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
			a[*rows * LA_TAM + (*cols)++] = atof(p);
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
			a[*rows * LA_TAM + (*cols)++] = atof(p);
		++*rows;
	}
	free(line);
}
