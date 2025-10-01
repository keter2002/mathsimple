/*
    base_orthonormalization - v1.0.3
    Makes the orthogonalization of a set of vectors. The given vectors need to
    be linearly independent.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.0.3  atof() replaces torfnum_atof()
        v1.0.2  torfnum_atof macro
        v1.0.1  stream arg in la_show_matrix_s()
        v1.0.0  First version

    base_orthonormalization is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

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
        u = &basis[i*LA_SIZE];
        for (j = i-1; j >= 0; j--) {
            g = &basis[j*LA_SIZE];
            inner_product = cblas_sdot(m, u, 1, g, 1);
            cblas_saxpy(m, -inner_product, g, 1, u, 1);
        }
        inner_product = cblas_sdot(m, u, 1, u, 1);
        cblas_sscal(m, 1/sqrt(inner_product), u, 1);
    }
}

main()
{
    float basis[LA_SIZE*LA_SIZE] = { 0 };
    int n, len, m;
    char *line, *p;
    size_t size;

    line = NULL;
    n = m = 0;
    while (getline(&line, &size, stdin) != EOF) {
        len = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            basis[n * LA_SIZE + len++] = atof(p);
        if (len > m)
            m = len;
        n++;
    }
    orthonormalization(basis, n, m);
    la_show_matrix_s(stdout, basis, n, m, LA_SIZE);
}
