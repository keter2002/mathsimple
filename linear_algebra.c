#include <string.h>
#include <stdlib.h>
#include "linear_algebra.h"

void show_matrix_s_la(a, rows, cols, lda)
float *a;
{
    int i, j;
    float aij;

    for (i=0; i < rows; i++, putchar('\n'))
        for (j=0; j < cols; j++) {
            aij = a[i * lda + j];
                PRINT_KNOW_CONSTANT_LA(aij)
            else
                printf("%f ", aij);
        }
}

void show_pointer_matrix_s_la(a, rows, cols)
float a[][SIZE_LA];
{
    int i;

    for (i=0; i < rows; i++)
        show_matrix_s_la(a[i], 1, cols, SIZE_LA);
}

void show_matrix_d_la(a, rows, cols, lda)
double *a;
{
    int i, j;
    double aij;

    for (i=0; i < rows; i++, putchar('\n'))
        for (j=0; j < cols; j++) {
            aij = a[i * lda + j];
                PRINT_KNOW_CONSTANT_LA(aij)
            else
                printf("%lf ", aij);
        }
}

void show_pointer_matrix_d_la(a, rows, cols)
double a[][SIZE_LA];
{
    int i;

    for (i=0; i < rows; i++)
        show_matrix_d_la(a[i], 1, cols, SIZE_LA);
}

extern double atof_torfnum();

void read_one_pointer_matrix_s_la(a, rows, cols)
float a[][SIZE_LA];
int *rows, *cols;
{
    char *line, *p;
    size_t size;

    *rows = size = 0;
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows][(*cols)++] = atof_torfnum(p);
        ++*rows;
    }
    free(line);
}

void read_one_pointer_matrix_d_la(a, rows, cols)
double a[][SIZE_LA];
int *rows, *cols;
{
    char *line, *p;
    size_t size;

    *rows = size = 0;
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows][(*cols)++] = atof_torfnum(p);
        ++*rows;
    }
    free(line);
}

void read_matrices_s_la(a, rows, cols)
float *a;
int *rows, *cols;
{
    char *line, *p;
    size_t size;
    
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows * SIZE_LA + (*cols)++] = atof_torfnum(p);
        ++*rows;
    }
    free(line);
}

void read_matrices_d_la(a, rows, cols)
double *a;
int *rows, *cols;
{
    char *line, *p;
    size_t size;
    
    line = NULL;
    while (getline(&line, &size, stdin) != EOF) {
        if (!strcmp(line, "e\n"))
            break;
        *cols = 0;
        for (p=strtok(line, " "); p; p = strtok(NULL, " "))
            a[*rows * SIZE_LA + (*cols)++] = atof_torfnum(p);
        ++*rows;
    }
    free(line);
}
