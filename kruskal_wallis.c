/*
    kruskal_wallis - v2.1.1
    Make the Kruskal-Wallis test. 
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.1.1  Wrong argument order when calling rank in H_test
        v2.1.0  Delimiter argument
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


ARRAYTYPED_GENERATE(double, 2, 0)

typedef arraytyped_array_double* group;

ARRAYTYPED_GENERATE(group, 2, 0)

arraytyped_array_double ranks;

/* Pode ser usada uma busca binária para acelerar a pesquisa. */
double rank(ranks, N, v)
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
double H_test(groups, c, ranks, N)
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
            sum += rank(ranks, N, sample->base[j]);
        rank_sum += sum*sum/r;
    }
    H *= rank_sum;
    return H-(3*N+3);
}

int arg_delimiter = ' ';
int arg_precision = 3;

main(argc, argv)
char *argv[];
{
    extern void la_show_matrix_d();
    void read_samples(), print_groups();
    int obs, i;

    arraytyped_array_group groups;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"delimiter", required_argument, NULL, 'd'},
        {"precision", required_argument, NULL, 'p'},
        {"verbose", no_argument, NULL, 'v'},
        { 0 },
    };
    int opt;
    int arg_verbose = 0;

    for (; (opt = getopt_long(argc, argv, "p:v", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'd':
            arg_delimiter = *optarg;
            break;
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
                  "  -d, --delimiter=DELIM   use DELIM instead of space for sample delimiter\n"
                  "  -p, --precision=NUM     printing precision of floating-point numbers, default\n"
                  "                          is 3\n"
                  "  -v, --verbose           prints the samples\n",
                  stdout);
            return 0;
        }

    arraytyped_allocate(group, groups, 2); 
    arraytyped_allocate(double, ranks, 10);

    read_samples(stdin, &groups, &obs);

    if (arg_verbose)
        print_groups(stdout, &groups, obs);

    qsort(ranks.base, ranks.nmemb, sizeof(double), mathfn_compar_double);
    printf("%.*lf\n", arg_precision, H_test(groups.base, groups.nmemb,
                                            ranks.base, ranks.nmemb));

    free(ranks.base);
    for (i=0; i < groups.nmemb; i++)
        free(groups.base[i]);
    free(groups.base);

    return 0;
}

void read_samples(stream, groups, obs)
FILE *stream;
arraytyped_array_group *groups;
int *obs;
{
    group tmp_group;
    char *line, *p, *c;
    size_t size;
    int j;
    double tmp;

    *obs = size = 0;
    line = NULL;
    while (getline(&line, &size, stream) != EOF) {
        j = 0;
        /* Like strtok but doesn't jump more than one delimiter at a time. */
        for (p=line, c=memchr(line, arg_delimiter, size); c;
              c=memchr(c, arg_delimiter, size-(c-line))) {
            if (j == groups->nmemb) {
                tmp_group = malloc(sizeof(arraytyped_array_double));
                arraytyped_append_to_end_ptr(group, groups, &tmp_group);
                arraytyped_allocate_ptr(group, tmp_group, 2);
            }
            *c='\0';
            if (*p) {
                tmp = atof(p);
                arraytyped_append_to_end(double, ranks, &tmp);
                arraytyped_append_to_end_ptr(double, groups->base[j], &tmp);
            }
            p = ++c;
            j++;
        }
        if (j == groups->nmemb) {
            tmp_group = malloc(sizeof(arraytyped_array_double));
            arraytyped_append_to_end_ptr(group, groups, &tmp_group);
            arraytyped_allocate_ptr(group, tmp_group, 2);
        }
        line[size-1] = '\0';
        if (*p) {
            tmp = atof(p);
            arraytyped_append_to_end(double, ranks, &tmp);
            arraytyped_append_to_end_ptr(double, groups->base[j], &tmp);
        }
        ++*obs;
    }
}

void print_groups(fp, groups, obs)
FILE *fp;
arraytyped_array_group *groups;
{
    int i,j;

    fputs("Groups:\n", fp);
    for (i=0; i < obs; i++, putchar('\n'))
        for (j=0; j < groups->nmemb; j++) {
            if (j) putchar(' ');
            if (i < groups->base[j]->nmemb)
                printf("%.*lf", arg_precision, groups->base[j]->base[i]);
            else
                printf("-");
        }
}
