/*
    logarithm - v1.0.0
    Calculates the natural logarithm of naturals.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    logarithm is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>
#include "torfnum.h"
#include "mathfn.h"

#define ITERATIONS 7

double log_e(x)
{
	double convergence, aux;
	int i, odd;
	
	for (convergence=0, x--; x; x--)
		for (aux = 2*x+1, i=0; i < ITERATIONS; i++) {
			odd = i*2+1;
			convergence += 2 * (1.0/odd) * (1.0/mathfn_powd(aux, odd));
		}
	return convergence;
}

/* Para números muito grandes, pode-se fatorar e calcular a soma dos logs dos
 * fatores. */
main(argc, argv)
char *argv[];
{
	int x;

	x = torfnum_atoi(argv[1]);
	if (x < 2) {
		fputs("Number must be greater than 1.\n", stderr);
		return 2;
	}
	printf("%f\n", log_e(x));
}
