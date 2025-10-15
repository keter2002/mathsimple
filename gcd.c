/*
    gcd - v2.0.2
    Get the greatest common divisor among arguments.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.2  Show help in stderr
        v2.0.1  Free gcd var
        v2.0.0  Support to arbitrary-precision integers
        v1.0.0  First version

    gcd is free software: you can redistribute it and/or modify it under the
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
    mpz_t *nums, gcd;
    int i;

    if (argc == 1) {
        fputs("Usage: gcd [NUMBER]...\n", stderr);
        return 2;
    }
    nums = malloc(sizeof(mpz_t) * (argc-1));
    for (i=1; i < argc; i++)
        mpz_init_set_str(nums[i-1], argv[i], 10);
    mpz_init(gcd);
    mpz_set(gcd, nums[0]);
    for (i=1; i < argc-1; i++)
        mpz_gcd(gcd, gcd, nums[i]);
    for (i=0; i < argc-1; i++)
        mpz_clear(nums[i]);
    free(nums);
    gmp_printf("%Zd\n", gcd);
    mpz_clear(gcd);
}
