/*
    binomial_series - v1.0.2
    Prints the first iterations of a binomial series (1+number)^exponent.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
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

main(argc, argv)
char *argv[];
{
    extern double mathfn_powd();
    double num, x, rt, numerator, m, aux, exp;
    int i, j, k, fat, it;

    if (argc < 3) {
        fputs("Usage: binomial_series number exponent iterations\n", stderr);
        return 2;
    }
    num = atof(argv[1]);
    exp = atof(argv[2]);
    it = argc > 3? atoi(argv[3]) : 0;
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
    for (j=1; j < it; j++) {
        numerator = m = exp;
        fat = 1;
        for (k=1; k < j; k++) {
            numerator *= m - k;    
            fat *= k+1;
        }
        m = numerator/fat;
        rt += m*mathfn_powd(x, j);
        printf("%f %f %f\n", rt, m, mathfn_powd(x,j));
    }
    printf("%.5lf\n", num > 1.0? rt*i : rt);
}
