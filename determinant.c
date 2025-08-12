#include <stdio.h>
#include "linear_algebra.h"

double determinant(m, rows, cols)
double m[][LA_TAM];
{
	int p[LA_TAM+1];
	int k_indexes[LA_TAM];
	int i, j, k, aux;

	double det = 0, prod;
	int sig = -1;
	// Realiza a primeira iteração do somatório:
	for (prod=m[0][0], k=1; k < cols; k++)
		prod *= m[k][k];
	det += prod;
	
	// QuickPerm Regressivo
	for (i=0; i < cols; i++) {
		k_indexes[i] = i;
		p[i] = i;
	}
	p[i] = i;
	i=1;
	while (i < cols) {
		p[i]--;
		j = i%2? p[i] : 0;
		
		// Troca
		aux = k_indexes[i];
		k_indexes[i] = k_indexes[j];
		k_indexes[j] = aux;
		
		// Realiza uma iteração do somatório.
		for (prod=m[0][k_indexes[0]], k=1; k < cols; k++)
			prod *= m[k][k_indexes[k]];
		det += sig*prod;
		sig *= -1;

		i = 1;
		while (!p[i]) {
			p[i] = i;
			i++;
		}
	}
	return det;
}

main()
{
	double matrix[LA_TAM][LA_TAM];
	int rows, cols;

	la_read_one_pointer_matrix_d(matrix, &rows, &cols);
	printf("%lf\n", determinant(matrix, rows, cols));
}
