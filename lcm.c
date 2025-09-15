/*
    lcm - v1.1.1
    Computes the least common multiple (LCM) of one or more arguments.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    lcm is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h> 
#include "torfnum.h"
#include "mathfn.h"

main(argc, argv)
char *argv[];
{
    mathfn_positive_int lcm, x;

    if (argc < 2) {
        fputs("lcm: a [b c ...]\n", stderr);
        return 2;
    }
    lcm = torfnum_atopi(argv[1]);
    while (argc > 2) {
        x = torfnum_atopi(argv[--argc]);
        lcm = MATHFN_LCM(lcm, x);
    }
    printf("%ld\n", lcm);
}
