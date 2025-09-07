/*
    gcd - v0.1.0
    Get the greatest common divisor among arguments.
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
#include "torfnum.h"
#include "mathfn.h"

main(argc, argv)
char *argv[];
{
    mathfn_positive_int *nums, gcd;
    int i;

    if (argc == 1) {
		puts("Usage: gcd a b c ...");
		return 2;
    }
    nums = malloc(sizeof(mathfn_positive_int) * (argc-1));
    for (i=1; i < argc; i++)
        nums[i-1] = torfnum_atopi(argv[i]);
    gcd = nums[0];
    for (i=1; i < argc-1; i++)
        gcd = mathfn_greatest_common_divisor(gcd, nums[i]);
    free(nums);
    printf("%lu\n", gcd);
}
