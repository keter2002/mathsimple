/*
    logarithm - v1.1.0
    Calculates the natural logarithm of naturals.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.1.0  Printing precision argument
        v1.0.2  Remove torfnum.h
        v1.0.1  atoi() replaces torfnum_atoi()
        v1.0.0  First version

    logarithm is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "mathfn.h"

#define ITERATIONS 7

double log_e(x)
{
    double convergence, aux;
    int i, odd;
    
    for (convergence=0, x--; x; x--)
        for (aux = 2*x+1, i=0; i < ITERATIONS; i++) {
            odd = i*2+1;
            convergence += 2 * (1.0/odd) * (1.0/mathfn_powd(aux, odd));
        }
    return convergence;
}

/* Para números muito grandes, pode-se fatorar e calcular a soma dos logs dos
 * fatores. */
main(argc, argv)
char *argv[];
{
    int x;
    
    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    int opt;
    int arg_precision = 6;

    for (; (opt = getopt_long(argc, argv, "p:", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case '?':
            fputs("Try 'logarithm --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: logarithm [OPTION] x\n"
                  "Calculates the natural logarithm of naturals.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 5\n" ,
                  stdout);
            return 0;
        }
    if (argc - optind < 1) {
        fputs("Need to inform x\n", stderr);
        return 2;
    }
    x = atoi(argv[optind]);
    if (x < 2) {
        fputs("Number must be greater than 1.\n", stderr);
        return 2;
    }

    printf("%.*f\n", arg_precision, log_e(x));
    return 0;
}
