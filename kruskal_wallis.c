#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "math.c"

#define MAX_GROUPS 5
#define MAX_OBS 50

float matrix[MAX_OBS][MAX_GROUPS];
float ranks[MAX_OBS*MAX_GROUPS];
int N;

/* Pode ser usada uma busca binária para acelerar a pesquisa. */
float rank(float v)
{
	int i, j;

	for (i=0; i < N; i++)
		if (ranks[i] == v) {
			for (j=i+1; j < N; j++)
				if (ranks[j] != v)
					break;
			return (j+i+1)/2.f;
		}
	fprintf(stderr, "%f not found.\n", v);
	exit(EXIT_FAILURE);
	return -1;
}

/* Estou assumindo que o número de observações em cada amostra é igual. */
float H_test(r, c)
{
	int i, j;
	float H, sum;
	float rank_sum;

	H = 12.f/(N*N+N);
	rank_sum = 0;
	for (i=0; i < c; i++) {
		sum = 0;
		for (j=0; j < r; j++)
			sum += rank(matrix[j][i]);
		rank_sum += sum*sum/r;
	}
	H *= rank_sum;
	return H-(3*N+3);
}

main()
{
	char *line, *p;
	size_t size;
	int rows, cols;
	int i,j;

	cols = rows = size = 0;
	line = NULL;
	N = 0;
	while (getline(&line, &size, stdin) != EOF) {
		cols = 0;
		for (p=strtok(line, " "); p; p = strtok(NULL, " ")) {
			ranks[N] = atof(p);
			matrix[rows][cols++] = ranks[N++];
		}
		rows++;
	}
	puts("Groups:");
	for (i=0; i < rows; i++, putchar('\n'))
		for (j=0; j < cols; j++)
			printf("%f ", matrix[i][j]);
	qsort(ranks, N, sizeof(float), compar_float);
	printf("%f\n", H_test(rows, cols));
}
