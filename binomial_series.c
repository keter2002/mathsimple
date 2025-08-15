/*
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
    extern double atof_torfnum();
    extern double powd_mathfn();
    extern atoi_torfnum(); 
    double num, x, rt, numerator, m, aux, exp;
    int i, j, k, fat, it;

    if (argc < 3) {
        fputs("Usage: binomial_series number exponent iterations\n", stderr);
        return 2;
    }
    num = atof_torfnum(argv[1]);
    exp = atof_torfnum(argv[2]);
    it = argc > 3? atoi_torfnum(argv[3]) : 0;
    if (num > 1.0) {
        i = 0;
        do {
            i++;
            aux = powd_mathfn((double)i, exp);
        } while (aux < num);
        if (!i) {
            fputs("Very small number.", stderr);
            return 1;
        }
        i -= 1;
        x = (num-powd_mathfn((double)i, exp))/powd_mathfn((double)i, exp);
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
        rt += m*powd_mathfn(x, j);
        printf("%f %f %f\n", rt, m, powd_mathfn(x,j));
    }
    printf("%.5lf\n", num > 1.0? rt*i : rt);
}
