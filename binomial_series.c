/*
    binomial_series - v1.0.0
    Prints the first iterations of a binomial series (1+number)^exponent.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

main(argc, argv)
char *argv[];
{
    extern double torfnum_atof();
    extern double mathfn_powd();
    extern torfnum_atoi(); 
    double num, x, rt, numerator, m, aux, exp;
    int i, j, k, fat, it;

    if (argc < 3) {
        fputs("Usage: binomial_series number exponent iterations\n", stderr);
        return 2;
    }
    num = torfnum_atof(argv[1]);
    exp = torfnum_atof(argv[2]);
    it = argc > 3? torfnum_atoi(argv[3]) : 0;
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
