#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linear_algebra.h"

#define LDA (LA_TAM<<1)

double matrix[LA_TAM*LDA];

main()
{
	char *line, *p;
	size_t size;
	int rows, cols;
	int i,j,k;

	cols = rows = size = 0;
	line = NULL;
	while (getline(&line, &size, stdin) != EOF) {
		cols = 0;
		for (p=strtok(line, " "); p; p = strtok(NULL, " "))
			matrix[LDA * rows + cols++] = atof(p);
		rows++;
	}
	// Concatenate with inverse:
	for (i=0; i < rows; i++)
		for (j=0; j < cols; j++)
			matrix[LDA * i + j + cols] = i==j;
	puts("M and I:");
	for (i=0; i < rows; i++, putchar('\n'))
		for (j=0; j < cols<<1; j++)
			printf("%f ", matrix[LDA * i + j]);
	for (k=0; k < rows; k++) {
		// Divide the row akj by akk.
		for (i=k+1; i < cols<<1; i++)
			matrix[LDA * k + i] /= matrix[LDA * k + k];
		// Ensures akk equals to one.
		matrix[LDA * k + k] = 1.f;
		// Subtract the next rows a(i=k+1)j by aik times akj.
		for (i=k+1; i < rows; i++) {
			for (j=k+1; j < cols<<1; j++)
				matrix[LDA * i + j] -= matrix[LDA * i + k] * matrix[LDA * k + j];
			// Ensures that aik equals to zero.
			matrix[LDA * i + k] = 0;
		}
		printf("Step %0d:\n", k+1);
		la_show_matrix_d(matrix, rows, cols, LDA);
	}
	// From the bottom up, subtract row akj times aik.
	for (k=rows-1; k > 0; k--)	
		for (i=k-1; i >= 0; i--) {
			for (j=k+1; j < cols<<1; j++)
				matrix[LDA * i + j] -= matrix[LDA * k + j] * matrix[LDA * i + k];
			// Guarantees aik equals to zero.
			matrix[LDA * i + k] = 0;
		}
	puts("M:");
	la_show_matrix_d(matrix, rows, cols, LDA);
	puts("M^-1:");
	la_show_matrix_d(&matrix[cols], rows, cols, LDA);
}
