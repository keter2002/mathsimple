/*
    Solves a quadratic formula with solutions in real numbers.
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
#include <math.h>

main(argc, argv)
char *argv[];
{
    extern double atof_torfnum();
    float a, b, c;
    float delta;

    if (argc < 4) {
        puts("Usage: bhaskara a b c");
        return 2;
    }
    a = atof_torfnum(argv[1]);
    b = -atof_torfnum(argv[2]);
    c = atof_torfnum(argv[3]);
    if ((delta = b*b - 4*a*c) < 0) {
        puts("No real solutions.");
        return 0;
    }
    a *= 2;
    printf("D:%f 2A:%f -B:%f C:%f\n", delta, a, b, c);
    printf("X1:%f X2:%f\n", (b + sqrt(delta)) / a, (b - sqrt(delta)) / a);
}
