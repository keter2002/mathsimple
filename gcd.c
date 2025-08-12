/*
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
    positive_int_mathfn *nums, gcd;
    int i;

    if (argc == 1) {
		puts("Usage: ./gcd a b c ...");
		return 2;
    }
    nums = malloc(sizeof(positive_int_mathfn) * (argc-1));
    for (i=1; i < argc; i++)
        nums[i-1] = atopi_torfnum(argv[i]);
    gcd = nums[0];
    for (i=1; i < argc-1; i++)
        gcd = greatest_common_divisor_mathfn(gcd, nums[i]);
    free(nums);
    printf("%lu\n", gcd);
}
