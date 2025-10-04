/*
    lcm - v2.0.1
    Computes the least common multiple (LCM) of one or more arguments.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.0.1  Remove torfnum.h
        v2.0.0  Support to arbitrary-precision integers
        v1.1.1  Fix wrong start of parsing args
        v1.1.0  Computes least common multiple of one or more arguments
        v1.0.0  First version

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

#include <gmp.h>

#include "mathfn.h"

main(argc, argv)
char *argv[];
{
    mpz_t lcm, x;
    int i;

    if (argc < 2) {
        fputs("Usage: lcm [NUMBER]...\n", stderr);
        return 2;
    }
    mpz_init_set_str(lcm, argv[1], 10);
    if (argc > 2)
        mpz_init(x);
    for (i=argc-1; i > 1; i--) {
        mpz_set_str(x, argv[i], 10);
        mpz_lcm(lcm, lcm, x);
    }
    if (argc > 2)
        mpz_clear(x);
    gmp_printf("%Zd\n", lcm);
    mpz_clear(lcm);
}
