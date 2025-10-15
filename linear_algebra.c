/*
    linear_algebra.c - v3.0.0
    Definitions of utility functions to deal with matrices.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v3.0.0  la_arg_precision and remove last space in la_show_matrix_*()
        v2.0.5  atof() replaces torfnum_atof()
        v2.0.4  torfnum_atof macro
        v2.0.3  Not exit on file know_constants.data missing
        v2.0.2  Remove __FILE__ from error messages in la_print_know_constant()
        v2.0.1  Shortens names of know_constant.h
        v2.0.0  la_print_know_constant(), stream args and lda in read_matrices
        v1.0.0  First version

    linear_algebra.c is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "linear_algebra.h"
#include "know_constant.h"


int la_arg_precision = 6;

static la_print_know_constant(stream, x)
FILE *stream;
double x;
{
    static unsigned char opened = 0;
    static arraytyped_array_know_constant_c know;
    static know_constant_c c;
    FILE *fp;
    know_constant_c *ptr;
    
    if (!opened) {
        /* File not found, so number not found. */
        if (!(fp = fopen(KNOW_CONSTANT_FILENAME, "rb")))
            return 1;
        if (fscanf(fp, "%lx\n", &know.nmemb) != 1)
            return 1;
        know.capacity = know.nmemb;
        know.base = malloc(sizeof(know_constant_c) * know.capacity);
        fread(know.base, sizeof(know_constant_c), know.nmemb, fp);
        fclose(fp);
        opened = 1;
    }
    c.val = x < 0? -x : x;
    if (!(ptr = arraytyped_find_know_constant_c(&know, &c)))
        return 1;
    fprintf(stream, "%s%s", x < 0? "-" : "", ptr->symb);
    return 0;
}

void la_show_matrix_s(stream, a, rows, cols, lda)
FILE *stream;
float *a;
{
    int i, j;
    float aij;

    for (i=0; i < rows; i++, putc('\n', stream))
        for (j=0; j < cols; j++) {
            aij = a[i * lda + j];
            if (la_print_know_constant(stream, aij))
                fprintf(stream, "%.*f", la_arg_precision, aij);
            if (j < cols-1)
                putc(' ', stream);
        }
}

void la_show_pointer_matrix_s(stream, a, rows, cols)
FILE *stream;
float a[][LA_SIZE];
{
    int i;
    
    for (i=0; i < rows; i++)
        la_show_matrix_s(stream, a[i], 1, cols, LA_SIZE);
}

void la_show_matrix_d(stream, a, rows, cols, lda)
FILE *stream;
double *a;
{
    int i, j;
    double aij;

    for (i=0; i < rows; i++, putc('\n', stream))
        for (j=0; j < cols; j++) {
            aij = a[i * lda + j];
            if (la_print_know_constant(stream, aij))
                fprintf(stream, "%.*lf", la_arg_precision, aij);
            if (j < cols-1)
                putc(' ', stream);
        }
}

void la_show_pointer_matrix_d(stream, a, rows, cols)
FILE *stream;
double a[][LA_SIZE];
{
    int i;
    
    for (i=0; i < rows; i++)
        la_show_matrix_d(stream, a[i], 1, cols, LA_SIZE);
}

void la_read_one_pointer_matrix_s(stream, a, rows, cols)
FILE *stream;
float a[][LA_SIZE];
int *rows, *cols;
{
    char *line, *p;
    size_t size;

    *rows = size = 0;
    line = NULL;
    while (getline(&line, &size, stream) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " ")) {
            assert(*cols < LA_SIZE);
            a[*rows][(*cols)++] = atof(p);
        }
        ++*rows;
    }
    free(line);
}

void la_read_one_pointer_matrix_d(stream, a, rows, cols)
FILE *stream;
double a[][LA_SIZE];
int *rows, *cols;
{
    char *line, *p;
    size_t size;

    *rows = size = 0;
    line = NULL;
    while (getline(&line, &size, stream) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " ")) {
            assert(*cols < LA_SIZE);
            a[*rows][(*cols)++] = atof(p);
        }
        ++*rows;
    }
    free(line);
}

void la_read_matrices_s(stream, a, rows, cols, lda)
FILE *stream;
float *a;
int *rows, *cols;
{
    char *line, *p;
    size_t size;
    
    line = NULL;
    while (getline(&line, &size, stream) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows * lda + (*cols)++] = atof(p);
        ++*rows;
    }
    free(line);
}

void la_read_matrices_d(stream, a, rows, cols, lda)
FILE *stream;
double *a;
int *rows, *cols;
{
    char *line, *p;
    size_t size;
    
    line = NULL;
    while (getline(&line, &size, stream) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows * lda + (*cols)++] = atof(p);
        ++*rows;
    }
    free(line);
}
