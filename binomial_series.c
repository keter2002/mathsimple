/*
    binomial_series - v2.0.1
    Prints the first iterations of a binomial series (1+number)^exponent.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.1  Doubly precision in printf
        v2.0.0  Printing precision and iterations arguments
        v1.0.2  Use atof() to replace torfnum_atof() and atoi() to replace
                torfnum_atoi()
        v1.0.1  torfnum_atof macro
        v1.0.0  First version

    binomial_series is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "args.h"


main(argc, argv)
char *argv[];
{
    extern double mathfn_powd();
    double num, x, rt, numerator, m, aux, exp;
    int i, j, k, fat;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"iterations", required_argument, NULL, 'i'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    char *args[] = {"number", "exponent"};
    int opt;
    int arg_it = 0;
    int arg_precision = 5;

    for (; (opt = getopt_long(argc, argv, "p:i:", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'i':
            arg_it = atoi(optarg);
            break;
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case '?':
            fputs("Try 'binomial_series --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: binomial_series [OPTION] number exponent iterations\n"
                  "Prints the first iterations of a binomial series (1+number)^exponent.\n\n"
                  "  -i, --iterations   total of iterations, default is 0\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 5\n",
                  stdout);
            return 0;
        }
    
    args_need_all_args(args);

    num = atof(argv[optind]);
    exp = atof(argv[optind+1]);
    if (num > 1.0) {
        i = 0;
        do {
            i++;
            aux = mathfn_powd((double)i, exp);
        } while (aux < num);
        if (!i) {
            fputs("Very small number.", stderr);
            return 1;
        }
        i -= 1;
        x = (num-mathfn_powd((double)i, exp))/mathfn_powd((double)i, exp);
    } else
        x = num;    
    rt = 1;
    for (j=1; j < arg_it; j++) {
        numerator = m = exp;
        fat = 1;
        for (k=1; k < j; k++) {
            numerator *= m - k;    
            fat *= k+1;
        }
        m = numerator/fat;
        rt += m*mathfn_powd(x, j);
        printf("%2$.*1$lf %3$.*1$lf %4$.*1$lf\n", arg_precision, rt, m,
               mathfn_powd(x,j));
    }
    printf("%.*lf\n", arg_precision, num > 1.0? rt*i : rt);
}
