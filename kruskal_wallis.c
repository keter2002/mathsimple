/*
    kruskal_wallis - v2.0.0
    Make the Kruskal-Wallis test. 
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.0.0  Support to samples with different sizes, doubly precision and
                command-line arguments
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

#include <getopt.h>

#include "external/arrays/array_typed.c"

#include "mathfn.h"


#define DELIMITER ' '

ARRAYTYPED_GENERATE(double, 2, 0)

typedef arraytyped_array_double* group;

ARRAYTYPED_GENERATE(group, 2, 0)

arraytyped_array_double ranks;
int N;

/* Pode ser usada uma busca binária para acelerar a pesquisa. */
double rank(ranks, v)
double *ranks, v;
{
    int i, j;

    for (i=0; i < N; i++)
        if (MATHFN_IS_EQUAL_LA(ranks[i], v)) {
            for (j=i+1; j < N; j++)
                if (!MATHFN_IS_EQUAL_LA(ranks[j], v))
                    break;
            return (j+i+1)/2.;
        }
    fprintf(stderr, "kruskal_wallis: %lf not found.\n", v);
    exit(EXIT_FAILURE);
}

/* Assumes that the number of observations in each sample is equal. */
double H_test(groups, c, ranks)
group *groups;
double *ranks;
{
    int i, j, r;
    double H, sum;
    double rank_sum;
    group sample;

    H = 12./(N*N+N);
    rank_sum = 0;
    for (i=0; i < c; i++) {
        sum = 0;
        sample = groups[i];
        r = sample->nmemb;
        for (j=0; j < r; j++)
            sum += rank(ranks, sample->base[j]);
        rank_sum += sum*sum/r;
    }
    H *= rank_sum;
    return H-(3*N+3);
}

main(argc, argv)
char *argv[];
{
    extern void la_show_matrix_d();
    char *line, *p, *c;
    size_t size;
    int obs, i, j;

    double tmp;
    group tmp_group;
    arraytyped_array_group groups;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"verbose", no_argument, NULL, 'v'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    int opt;
    int arg_precision = 3;
    int arg_verbose = 0;

    for (; (opt = getopt_long(argc, argv, "p:v", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case 'v':
            arg_verbose = 1;
            break;
        case '?':
            fputs("Try 'kruskal_wallis --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: kruskal_wallis [OPTION]...\n"
                  "Make the Kruskal-Wallis test.\n\n"
                  "  -v, --verbose      prints the samples\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 1\n",
                  stdout);
            return 0;
        }

    arraytyped_allocate(group, groups, 2); 
    arraytyped_allocate(double, ranks, 10);
    obs = size = 0;
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        j = 0;
        /* Like strtok but doesn't jump more than one delimiter at a time. */
        for (p=line, c=memchr(line, DELIMITER, size); c;
              c=memchr(c, DELIMITER, size-(c-line))) {
            if (j == groups.nmemb) {
                tmp_group = malloc(sizeof(arraytyped_array_double));
                arraytyped_append_to_end(group, groups, &tmp_group);
                arraytyped_allocate_ptr(group, tmp_group, 2);
            }
            *c='\0';
            if (*p) {
                tmp = atof(p);
                arraytyped_append_to_end(double, ranks, &tmp);
                arraytyped_append_to_end_ptr(double, groups.base[j], &tmp);
            }
            p = ++c;
            j++;
        }
        if (j == groups.nmemb) {
            tmp_group = malloc(sizeof(arraytyped_array_double));
            arraytyped_append_to_end(group, groups, &tmp_group);
            arraytyped_allocate_ptr(group, tmp_group, 2);
        }
        line[size-1] = '\0';
        if (*p) {
            tmp = atof(p);
            arraytyped_append_to_end(double, ranks, &tmp);
            arraytyped_append_to_end_ptr(double, groups.base[j], &tmp);
        }
        obs++;
    }
    N = ranks.nmemb;

    if (arg_verbose) {
        puts("Groups:");
        for (i=0; i < obs; i++, putchar('\n'))
            for (j=0; j < groups.nmemb; j++) {
                if (j) putchar(' ');
                if (i < groups.base[j]->nmemb)
                    printf("%.*lf", arg_precision, groups.base[j]->base[i]);
                else
                    printf("-");
            }
    }
    qsort(ranks.base, N, sizeof(double), mathfn_compar_double);
    printf("%.*lf\n", arg_precision, H_test(groups.base, groups.nmemb, ranks.base));

    free(ranks.base);
    for (i=0; i < groups.nmemb; i++)
        free(groups.base[i]);
    free(groups.base);

    return 0;
}
