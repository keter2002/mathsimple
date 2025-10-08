/*
    find_know_number - v2.0.2
    Return the symbolic representation of a floating point number.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.2  Return 0 in main if help arg
        v2.0.1  Use atof() to replace torfnum_atof() and atoi() to replace
                torfnum_atoi()
        v2.0.0  Search for know mathematical constants and parameters of search
        v1.0.0  First version

    find_know_number is free software: you can redistribute it and/or modify it
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

#include "know_constant.h"
#include "mathfn.h"

#define RANGE 500
#define KNOW_CONSTANT_RANGE 30


main(argc, argv)
char *argv[];
{
    int find_number(), find_know_contant();
    int range, know_constant_range;
    double epsilon;
    unsigned char found;

    unsigned char suppress_number_finding = 0;
    unsigned char suppress_know_constant_finding = 0;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"epsilon", required_argument, NULL, 'e'},
        {"range", required_argument, NULL, 0},
        {"know_constant_range", required_argument, NULL, 0},
        { 0 },
    };
    int opt, option_index;
    
    if (argc < 2)
        goto help;
    
    epsilon = LA_EPS;
    range = RANGE;
    know_constant_range = KNOW_CONSTANT_RANGE;

    for (; (opt = getopt_long(argc, argv, "-12e:", long_opts, &option_index)) != -1;)
        switch (opt) {
        case 0:
            if (!strcmp("range", long_opts[option_index].name))
                range = atoi(optarg);
            else
                know_constant_range = atoi(optarg);
            break;
        case 1:
            found = 0;
            if (!suppress_number_finding)
                found = find_number(stdout, optarg, range, epsilon);
            if (!suppress_know_constant_finding)
                found |= find_know_contant(stdout, optarg, know_constant_range, epsilon);
            if (!found)
                printf("%s not found\n", optarg);
            break;
        case '1':
            suppress_number_finding = 1;
            break;
        case '2':
            suppress_know_constant_finding = 1;
            break;
        case 'e':
            epsilon = atof(optarg);
            break;
        case 'h':
        case '?':
help:
            fputs("Usage: find_know_number [OPTION]... [NUMBER]...\n"
                  "Return the symbolic representation of a floating point number.\n\n"
                  "By default searches with LA_EPS precision if the number is found in a fraction,\n"
                  "square root or a number squared. It searches through the mathematical\n"
                  "constants defined in math lib, with same transformations.\n\n"
                  "  -1         Do not search for integers transformed by the operators supported.\n"
                  "  -2         Do not search for constants like pi, e, etc transformed by the\n"
                  "             operators supported.\n\n"
                  "  -e, --epsilon         Inform the precision to look for.\n"
                  "      --range           Range that fractions will go for integers kind.\n"
                  "                        Example, if informed 10, search from 1/2 to 1/10.\n"
                  "      --know_constant_range    Range that the fractions will go for the\n"
                  "                               constants.\n",
                  stderr);
            return opt == 'h'? 0 : 2;
        }

    return 0;
}

find_number(stream, strx, range, eps)
FILE *stream;
char *strx;
double eps;
{
    int i, j, k;
    double ilf, jlf, klf;
    double sqrt_lf, c, x;
    unsigned char found = 0;
    
    x = atof(strx);
    for (i=1; i <= range; i++) {
        ilf = i;
        sqrt_lf = sqrt(ilf);
        if (ceil(sqrt_lf) != sqrt_lf) {
            c = sqrt(ilf);
            if (MATHFN_IS_EQUAL(x,c,eps)) {
                printf("%s = %d^(1/2)\n", strx, i);
                found = 1;
            }
        }
        for (j=2; j <= range; j++) {
            jlf = j;
            if (mathfn_greatest_common_divisor(i, j) == 1) {
                c = i / jlf;
                if (MATHFN_IS_EQUAL(x,c,eps)) {
                    printf("%s = %d/%d\n", strx, i, j);
                    found = 1;
                }
            }
            if (ceil(sqrt_lf) != sqrt_lf) {
                c = sqrt(ilf) * j;
                if (MATHFN_IS_EQUAL(x,c,eps)) {
                    printf("%s = %d.%d^(1/2)\n", strx, j, i);
                    found = 1;
                }
                c = sqrt(ilf) / jlf;
                if (MATHFN_IS_EQUAL(x,c,eps)) {
                    printf("%s = %d^(1/2)/%d\n", strx, i, j);
                    found = 1;
                }
                for (k=2; k <= range; k++)
                    if (mathfn_greatest_common_divisor(j, k) == 1) {
                        klf = k;
                        c = sqrt(ilf)*j/klf;
                        if (MATHFN_IS_EQUAL(x,c,eps)) {
                            printf("%s = %d.%d^(1/2)/%d\n", strx, j, i, k);
                            found = 1;
                        }
                    }
            }
        }
    }
    return found;
}

find_know_contant(stream, strx, range, eps)
FILE *stream;
char *strx;
double eps;
{
    int i, j, k;
    double jlf, klf;
    double sqrt_lf, c, x;
    double val, squared;
    char *symb;
    unsigned char found = 0;

    x = atof(strx);
    for (i=0; i < sizeof(know_constant_constants) /
                  sizeof(know_constant_c) - 1; i++) {
        val = know_constant_constants[i].c.val;
        symb = know_constant_constants[i].c.symb;

        c = val;
        if (MATHFN_IS_EQUAL(x,c,eps)) {
            printf("%s = %s\n", strx, symb);
            found = 1;
        }
        /* SQRT */
        c = sqrt_lf = sqrt(val);
        if (MATHFN_IS_EQUAL(x,c,eps)) {
            printf("%s = (%s)^(1/2)\n", strx, symb);
            found = 1; 
        }
        /* SQUARED */
        c = squared = val*val;
        if (MATHFN_IS_EQUAL(x,c,eps)) {
            printf("%s = (%s)^2\n", strx, symb);
            found = 1; 
        }
        for (j=2; j <= range; j++) {
            jlf = j;
            /* OVER */
            c = val / jlf;
            if (MATHFN_IS_EQUAL(x,c,eps)) {
                printf("%s = %s/%d\n", strx, symb, j);
                found = 1;
            }
            /* SQRT_TIMES */
            c = sqrt_lf * j;
            if (MATHFN_IS_EQUAL(x,c,eps)) {
                printf("%s = %d.(%s)^(1/2)\n", strx, j, symb);
                found = 1; 
            }
            /* SQRT_OVER */
            c = sqrt_lf / jlf;
            if (MATHFN_IS_EQUAL(x,c,eps)) {
                printf("%s = (%s)^(1/2)/%d\n", strx, symb, j);
                found = 1; 
            }
            /* SQUARED_TIMES */
            c = squared * j;
            if (MATHFN_IS_EQUAL(x,c,eps)) {
                printf("%s = %d.(%s)^2\n", strx, j, symb);
                found = 1; 
            }
            /* SQUARED_OVER */
            c = squared / jlf;
            if (MATHFN_IS_EQUAL(x,c,eps)) {
                printf("%s = (%s)^2/%d\n", strx, symb, j);
                found = 1; 
            }
        }
        for (j=2; j <= range; j++)
            for (k=2; k <= range; k++)
                if (mathfn_greatest_common_divisor(j, k) == 1) {
                    klf = k;
                    /* TIMES_OVER */
                    c = val*j/klf;
                    if (MATHFN_IS_EQUAL(x,c,eps)) {
                        printf("%s = %d.%s/%d\n", strx, j, symb, k);
                        found = 1; 
                    }
                    /* SQRT_TIMES_OVER */
                    c = sqrt_lf*j/klf;
                    if (MATHFN_IS_EQUAL(x,c,eps)) {
                        printf("%s = %d.(%s)^(1/2)/%d\n", strx, j, symb, k);
                        found = 1; 
                    }
                    /* SQUARED_TIMES_OVER */
                    c = squared*j/klf;
                    if (MATHFN_IS_EQUAL(x,c,eps)) {
                        printf("%s = %d.(%s)^2/%d\n", strx, j, symb, k);
                        found = 1; 
                    }
                }
    }
    return found;
}
