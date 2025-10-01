/*
    bhaskara - v1.0.1
    Solves a quadratic formula with solutions in real numbers.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
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

#include "torfnum.h"

main(argc, argv)
char *argv[];
{
    float a, b, c;
    float delta;

    if (argc < 4) {
        puts("Usage: bhaskara a b c");
        return 2;
    }
    a = torfnum_atof(argv[1]);
    b = -torfnum_atof(argv[2]);
    c = torfnum_atof(argv[3]);
    if ((delta = b*b - 4*a*c) < 0) {
        puts("No real solutions.");
        return 0;
    }
    a *= 2;
    printf("D:%f 2A:%f -B:%f C:%f\n", delta, a, b, c);
    printf("X1:%f X2:%f\n", (b + sqrt(delta)) / a, (b - sqrt(delta)) / a);
}
