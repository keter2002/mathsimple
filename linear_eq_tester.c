#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "linear_algebra.h"

double matrix[LA_TAM][LA_TAM];
double values[LA_TAM];

main()
{
	char *line, *p;
	size_t size;
	int rows, cols;
	int i,j,k;
	double sum;

	cols = rows = size = 0;
	line = NULL;
	while (getline(&line, &size, stdin) != EOF) {
		if (!strcmp(line, "e\n"))
			break;
		cols = 0;
		for (p=strtok(line, " "); p; p = strtok(NULL, " "))
			matrix[rows][cols++] = atof(p);
		rows++;
	}
	puts("Linear System:");
	for (i=0; i < rows; i++, putchar('\n'))
		for (j=0; j < cols; j++)
			printf("%f ", matrix[i][j]);
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
