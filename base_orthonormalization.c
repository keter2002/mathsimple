#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "cblas.h"
#include "linear_algebra.h"

void orthonormalization(basis, n, m)
float *basis;
{
	int i, j;
	float inner_product, *u, *g;
	
	for (i=0; i < n; i++) {
		u = &basis[i*LA_TAM];
		for (j = i-1; j >= 0; j--) {
			g = &basis[j*LA_TAM];
			inner_product = cblas_sdot(m, u, 1, g, 1);
			cblas_saxpy(m, -inner_product, g, 1, u, 1);
		}
		inner_product = cblas_sdot(m, u, 1, u, 1);
		cblas_sscal(m, 1/sqrt(inner_product), u, 1);
	}
}

main()
{
	float basis[LA_TAM*LA_TAM] = { 0 };
	int n, len, m;
	char *line, *p;
	size_t size;
	
	line = NULL;
	n = m = 0;
	while (getline(&line, &size, stdin) != EOF) {
		len = 0;
		for (p=strtok(line, " "); p; p = strtok(NULL, " "))
			basis[n * LA_TAM + len++] = atof(p);
		if (len > m)
			m = len;
		n++;
	}
	orthonormalization(basis, n, m);
	la_show_matrix_s(basis, n, m, LA_TAM);
}
