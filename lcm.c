/*
    lcm - v1.0.0
    Computes the least common multiple (LCM) of two arguments.
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
	int x,y;

	if (argc < 3) {
		fputs("lcm: x y\n", stderr);
		return 2;
	}
	x = torfnum_atoi(argv[1]);
	y = torfnum_atoi(argv[2]);
	printf("%ld\n", MATHFN_LCM(x,y));
}
