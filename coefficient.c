/*
    coefficient - v2.0.1
    Computes the Pearson's correlation coefficient (r) and fits a line by
    linear regression.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.1  Help arg
        v2.0.0  Translate to english
        v1.0.0  First version

    coefficient is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

#include "getop.c"
#include "external/arrays/array_typed.c"

#define TRUE 1
#define FALSE 0
#define LEFT 0
#define RIGHT 1
#define DELIMITER ';'
#define STEP 2

typedef double point[2];

ARRAYTYPED_GENERATE(point, STEP, 0)

int columns = FALSE;

int main(argc, argv)
int argc;
char *argv[];
{
    void read_nums();
    FILE *fp;
    arraytyped_array_point points;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"column", no_argument, NULL, 'c'},
        { 0 },
    };
    int opt;
    
    switch ((opt = getopt_long(argc, argv, "c", long_opts, NULL))) {
    case '?':
        fputs("Try 'coefficient --help' for more information.\n", stderr);
        return 2;
    case 'c':
        columns = TRUE; 
        argc--;
        argv++;
        break;
    case 'h':
        fputs("Usage: coefficient [OPTION] [FILE]...\n"
              "Computes the Pearson's correlation coefficient (r) and fits a line by linear\n"
              "regression.\n\n"
              "With no FILE read standard input.\n\n"
              "  -c, --column    a column is a coordinate\n",
              stderr);
        return 0;
    }
    arraytyped_allocate(point, points, 1);
    if (argc <= 1)
        read_nums(stdin, &points);
    else
        while (--argc > 0) {
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "coefficient: file not found (%s)\n", *argv);
                return 1;
            }
            read_nums(fp, &points);
            fclose(fp);
        }
    return 0;
}

static first;

void read_nums(fp, arr)
FILE *fp;
arraytyped_array_point *arr;
{
    extern double atof_torfnum();
    void insert_tuple(), estatistics();
    char s[MAXOP];
    int type, side = LEFT;

    arr->nmemb = 0;
    first = FALSE;
    while ((type = getop(s, MAXOP, fp)) != EOF)
        if (type == NUMBER) {
            insert_tuple(atof(s), arr, side);
            if (columns == FALSE)
                side = !side;
        } else if (type == DELIMITER && columns)
            side = RIGHT;
    estatistics(arr);
    putchar('\n');
}

void insert_tuple(x, arr, side)
double x;
arraytyped_array_point *arr;
{
    if (side == RIGHT && columns && first == FALSE) {
        arr->nmemb = 0;
        first = TRUE;
    }
    arraytyped_expand_point(arr);
    (*ARRAYTYPED_LAST_SPACE_PTR(arr))[side] = x;
    if ((columns==FALSE && side==RIGHT) || columns)
        arr->nmemb++;
}

void estatistics(v)
arraytyped_array_point *v;
{
    double (*ptr)[2];
    double sumx, sumy, sumx2, sumy2, sumxy;
    double r, a, b, nume, deno;
    int n = v->nmemb;
    char *correlation;
    
    sumx = sumy = sumx2 = sumy2 = sumxy = .0;
    for (ptr=v->base; ptr < v->base + n; ptr++) {
        sumx += (*ptr)[LEFT];
        sumy += (*ptr)[RIGHT];
        sumx2 += pow((*ptr)[LEFT], 2);
        sumy2 += pow((*ptr)[RIGHT], 2);
        sumxy += (*ptr)[LEFT] * (*ptr)[RIGHT];
    }
    nume = n*sumxy-sumx*sumy;
    deno = n*sumx2-pow(sumx,2);
    printf("%.3lf %.3lf %.3lf %.3lf %.3lf %.3lf %.3lf\n", nume, deno, sumx, sumx2, sumy, sumy2, sumxy);

    b = nume/deno;
    a = sumy/n - b*(sumx/n);
    r = nume/sqrt(deno*(n*sumy2-pow(sumy,2)));
    printf("r: %.4lf\na: %.2lf\tb: %.2lf\n", trunc(r*10000)/10000, a, b);

    r = fabs(r);
    if (r == .0)
        correlation = "null";
    else if (r < .3)
        correlation = "weak";
    else if (r < .6)
        correlation = "regular";
    else if (r < .9)
        correlation = "strong";
    else if (r < 1.0)
        correlation = "very strong";
    else if (r == 1.0)
        correlation = "perfect";
    printf("The correlation is %s.", correlation);
}
