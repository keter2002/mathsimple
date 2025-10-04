/*
    kruskal_wallis - v1.0.3
    Make the Kruskal-Wallis test. 
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.0.3  remove torfnum.h
        v1.0.2  atof() replaces torfnum_atof()
        v1.0.1  torfnum_atof macro
        v1.0.0  First version

    kruskal_wallis is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mathfn.h"

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

/* Assumes that the number of observations in each sample is equal. */
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
    extern void la_show_matrix_s();
    char *line, *p;
    size_t size;
    int rows, cols;

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
    la_show_matrix_s(matrix, rows, cols, MAX_GROUPS);
    qsort(ranks, N, sizeof(float), mathfn_compar_float);
    printf("%f\n", H_test(rows, cols));
}
