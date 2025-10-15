/*
    mathfn.c - v1.0.0
    Mathematical functions definitions.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    mathfn.c is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <assert.h>
#include <math.h>

#include "mathfn.h"


mathfn_positive_int mathfn_greatest_common_divisor(x, y)
mathfn_positive_int x, y;
{
    while (x && y)
        if (x > y)
            x %= y;
        else
            y %= x;
    return x? x : y;
}

unsigned mathfn_brent(f, end, mu)
long *f;
unsigned *mu;
{
    long tortoise, hare;
    unsigned hare_i, tortoise_i, power, lam;
    
    /* Needs at least 2 in array f. */
    assert(end>1);

    power = lam = 1;
    tortoise = f[0];
    hare_i = 1;
    hare = f[1];
    while (tortoise != hare) {
        if (power == lam) {
            tortoise = hare;
            power<<=1;
            lam = 0;
        }
        if (++hare_i == end)
            return 0;
        hare = f[hare_i];
        lam++;
    }
    
    tortoise = f[0];
    tortoise_i = 0;
    hare = f[lam];
    hare_i = lam;
    for (*mu = 0; tortoise != hare; ++*mu) {
        tortoise = f[tortoise_i++];
        hare = f[hare_i++];
    }
    return lam;
}

/* Only for n >= 0 and integer. */
mathfn_positive_int mathfn_powi(a, n)
mathfn_positive_int a, n;
{
    mathfn_positive_int b = 1;

    while (n--)
        b *= a;
    return b;
}
double mathfn_powd(a, n)
double a;
mathfn_positive_int n;
{
    double b = 1;
    
    while (n--)
        b *= a;
    return b;
}

mathfn_compar_double(x, y)
const void *x, *y;
{
    double d;
    
    d = *(double*)x-*(double*)y;
    if (fabs(d) <= LA_EPS)
        return 0;
    return d > 0? 1 : -1;
}
mathfn_compar_float(x, y)
const void *x, *y;
{
    float d;
    
    d = *(float*)x-*(float*)y;
    if (fabsf(d) <= LA_EPS)
        return 0;
    return d > 0? 1 : -1;
}

double mathfn_roundd(x)
double x;
{
    return (x > 0? (int)(100*x+0.5)/100.0 : (int)(100*x-0.5)/100.0);
}
