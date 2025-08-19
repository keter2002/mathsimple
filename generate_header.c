/*
    Program to generate LA_PRINT_KNOW_CONSTANT macro. This macro gets a
    floating point number and prints equivalent mathematical symbol.
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
#include <math.h>
#include "mathfn.h"

#define RANGE 13

void print_elifs(char *sig)
{
	int i,j,k;
	double ilf, sqrt_i;

	for (i=1; i <= RANGE; i++) {
		ilf = i;
		sqrt_i = sqrt(ilf);
		if (ceil(sqrt_i) != sqrt_i)
			printf(" else if (%sLA_SQRT_%02d - LA_EPS < x && %sLA_SQRT_%02d + LA_EPS > x) { \\\n"
				"    printf(\"%s%d^(1/2) \");\\\n"
				"}", sig, i, sig, i, sig, i);
		for (j=2; j <= RANGE; j++) {
			if (mathfn_greatest_common_divisor(i, j) == 1)
				printf(" %sif (%sLA_%02d_OVER_%02d - LA_EPS < x && %sLA_%02d_OVER_%02d + LA_EPS > x) { \\\n"
					"    printf(\"%s%d/%d \");\\\n"
					"}", i==1 && j==2 && !*sig? "" : "else ", sig, i, j, sig, i, j, sig, i, j);
			if (ceil(sqrt_i) != sqrt_i) {
				/* SQRT_TIMES */
				printf(" else if (%sLA_SQRT_%02d_TIMES_%02d - LA_EPS < x && %sLA_SQRT_%02d_TIMES_%02d + LA_EPS > x) { \\\n"
					"    printf(\"%s%d.%d^(1/2) \");\\\n"
					"}", sig, i, j, sig, i, j, sig, j, i);
				/* SQRT_OVER */
				printf(" else if (%sLA_SQRT_%02d_OVER_%02d - LA_EPS < x && %sLA_SQRT_%02d_OVER_%02d + LA_EPS > x) { \\\n"
					"    printf(\"%s%d^(1/2)/%d \");\\\n"
					"}", sig, i, j, sig, i, j, sig, i, j);
				for (k=2; k <= RANGE; k++)
					if (mathfn_greatest_common_divisor(j, k) == 1)
						printf(" else if (%sLA_SQRT_%02d_TIMES_%02d_OVER_%02d - LA_EPS < x && %sLA_SQRT_%02d_TIMES_%02d_OVER_%02d + LA_EPS > x) { \\\n"
							"    printf(\"%s%d.%d^(1/2)/%d \");\\\n"
							"}", sig, i, j, k, sig, i, j, k, sig, j, i, k);
			}
		}
	}
}

main()
{
	int i, j, k;
	double ilf, jlf, klf;
	double sqrt_i;
	
	puts("#ifndef LA_PRINT_KNOW_CONSTANT_H");
	puts("#define LA_PRINT_KNOW_CONSTANT_H\n");
	/* Frações.	 */
	for (i=1; i <= RANGE; i++) {
		ilf = i;
		sqrt_i = sqrt(ilf);
		if (ceil(sqrt_i) != sqrt_i)
			printf("#define LA_SQRT_%02d %.14lf\n", i, sqrt(ilf));
		for (j=2; j <= RANGE; j++) {
			jlf = j;
			if (mathfn_greatest_common_divisor(i, j) == 1)
				printf("#define LA_%02d_OVER_%02d %.14lf\n", i, j, i/jlf);
			if (ceil(sqrt_i) != sqrt_i) {
				printf("#define LA_SQRT_%02d_TIMES_%02d %.14lf\n", i, j, sqrt(ilf) * j);
				printf("#define LA_SQRT_%02d_OVER_%02d %.14lf\n", i, j, sqrt(ilf) / jlf);
				for (k=2; k <= RANGE; k++)
					if (mathfn_greatest_common_divisor(j, k) == 1) {
						klf = k;
						printf("#define LA_SQRT_%02d_TIMES_%02d_OVER_%02d %.14lf\n", i, j, k, sqrt(ilf)*j/klf);
					}
			}
		}
	}
	puts("\n#define LA_PRINT_KNOW_CONSTANT(x)\\");
	print_elifs("");
	print_elifs("-");
	puts("\n\n#endif");
}
