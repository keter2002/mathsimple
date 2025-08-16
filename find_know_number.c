/*
    Return the symbolic representation of a floating point number.
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
#include "eps_la.h"
#include "torfnum.h"
#include "mathfn.h"

#define RANGE 1000

main(argc, argv)
char *argv[];
{
	int i, j, k;
	double ilf, jlf, klf;
	double sqrt_i;
	double x, c;
	
	x = atof_torfnum(argv[1]);

	for (i=1; i <= RANGE; i++) {
		ilf = i;
		sqrt_i = sqrt(ilf);
		if (ceil(sqrt_i) != sqrt_i) {
			c = sqrt(ilf);
			if (c - EPS_LA < x && c + EPS_LA > x) {
				printf("%d^(1/2)\n", i);
				return 0;
			}
		}
		for (j=2; j <= RANGE; j++) {
			jlf = j;
			if (greatest_common_divisor_mathfn(i, j) == 1) {
				c = i / jlf;
				if (c - EPS_LA < x && c + EPS_LA > x) {
					printf("%d/%d\n", i, j);
					return 0;
				}
			}
			if (ceil(sqrt_i) != sqrt_i) {
				c = sqrt(ilf) * j;
				if (c - EPS_LA < x && c + EPS_LA > x) {
					printf("%d.%d^(1/2)\n", j, i);
					return 0;
				}
				c = sqrt(ilf) / jlf;
				if (c - EPS_LA < x && c + EPS_LA > x) {
					printf("%d^(1/2)/%d\n", i, j);
					return 0;
				}
				for (k=2; k <= RANGE; k++)
					if (greatest_common_divisor_mathfn(j, k) == 1) {
						klf = k;
						c = sqrt(ilf)*j/klf;
						if (c - EPS_LA < x && c + EPS_LA > x) {
							printf("%d.%d^(1/2)/%d\n", j, i, k);
							return 0;
						}
					}
			}
		}
	}
}
