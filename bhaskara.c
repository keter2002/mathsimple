/*
    bhaskara - v1.2.0
    Solves a quadratic formula with solutions in real numbers.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.2.0  Doubly precision
        v1.1.0  Printing precision argument
        v1.0.3  remove torfnum.h
        v1.0.2  atof() replaces torfnum_atof()
        v1.0.1  torfnum_atof macro
        v1.0.0  First version

    bhaskara is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
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

#include "args.h"


main(argc, argv)
char *argv[];
{
    double a, b, c;
    double delta;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    char *args[] = {"a", "b", "c"};
    int opt,i;
    int arg_precision = 6;

    for (; (opt = getopt_long(argc, argv, "p:i:", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case '?':
            fputs("Try 'bhaskara --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: bhaskara [OPTION] a b c\n"
                  "Solves a quadratic formula with solutions in real numbers.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n",
                  stdout);
            return 0;
        }
    
    args_need_all_args(args);

    a = atof(argv[optind]);
    b = -atof(argv[optind+1]);
    c = atof(argv[optind+2]);
    if ((delta = b*b - 4*a*c) < 0) {
        puts("No real solutions.");
        return 0;
    }
    a *= 2;
    printf("D:%2$.*1$lf 2A:%3$.*1$lf -B:%4$.*1$lf C:%5$.*1$lf\n",
           arg_precision, delta, a, b, c);
    printf("X1:%2$.*1$lf X2:%3$.*1$lf\n", arg_precision, (b + sqrt(delta)) / a,
           (b - sqrt(delta)) / a);

    return 0;
}
