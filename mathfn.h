/*
    mathfn.h - v1.1.0
    Mathematical functions declarations.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.1.0  MATHFN_IS_EQUAL macro
        v1.0.1  Capitalize macro parameters in MATHFN_LCM
        v1.0.0  First version

    mathfn.h is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef MATHFN_H
#define MATHFN_H

#include "la_eps.h"

typedef unsigned long mathfn_positive_int;

#define MATHFN_IS_EQUAL(X,Y,EPSILON) ((Y) - (EPSILON) < (X) && (Y) + (EPSILON) > (X))
#define MATHFN_IS_EQUAL_LA(X,Y) MATHFN_IS_EQUAL(X,Y,LA_EPS)

#define MATHFN_LCM(X,Y) ((X) * (Y) / mathfn_greatest_common_divisor(X, Y))

unsigned mathfn_brent(long *f, int end, unsigned *mu);

mathfn_positive_int mathfn_greatest_common_divisor(mathfn_positive_int x, mathfn_positive_int y);
/* Only for n >= 0 and integer. */
mathfn_positive_int mathfn_powi(mathfn_positive_int a, mathfn_positive_int n);
double mathfn_powd(double a, mathfn_positive_int n);

int mathfn_compar_double(const void *x, const void *y);
int mathfn_compar_float(const void *x, const void *y);

double mathfn_roundd(double x);

#endif
