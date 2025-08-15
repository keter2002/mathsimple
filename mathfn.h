/*
    Mathematical functions declarations.
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

#ifndef H_MATHFN
#define H_MATHFN


typedef unsigned long positive_int_mathfn;

positive_int_mathfn greatest_common_divisor_mathfn(positive_int_mathfn x, positive_int_mathfn y);

#define MMC_MATHFN(X,Y) ((X) * (Y) / greatest_common_divisor_mathfn(x,y))

unsigned brent_mathfn(long *f, int end, unsigned *mu);

/* Only for n >= 0 and integer. */
positive_int_mathfn powi_mathfn(positive_int_mathfn a, positive_int_mathfn n);
double powd_mathfn(double a, positive_int_mathfn n);

int compar_double_mathfn(const void *x, const void *y);
int compar_float_mathfn(const void *x, const void *y);

double roundd_mathfn(double x);

#endif
