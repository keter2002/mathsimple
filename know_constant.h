/*
    know_constant.h - v1.0.0
    Header file of know_constant.c.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    know_constant.h is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef KNOW_CONSTANT_H
#define KNOW_CONSTANT_H

#include <math.h>

#include "external/arrays/array_typed.c"

#define KNOW_CONSTANT_RANGE 13

#define KNOW_CONSTANT_MAX_SYMB_LEN 30

#define KNOW_CONSTANT_FILENAME "know_constants.data"

typedef struct {
    double val;
    char symb[KNOW_CONSTANT_MAX_SYMB_LEN];
} know_constant_c;

typedef struct {
    char *str;
    know_constant_c c;
} know_constant_define;

know_constant_define know_constant_constants[] =
    {{"M_E", {M_E, "e"}},
     {"M_LOG2E", {M_LOG2E, "log_2(e)"}},
     {"M_LOG10E", {M_LOG10E, "log_10(e)"}},
     {"M_LN2", {M_LN2, "ln(2)"}},
     {"M_LN10", {M_LN10, "ln(10)"}},
     {"M_PI", {M_PI, "pi"}}};

know_constant_compar_math_constant(x, y)
void *x, *y;
{
    extern mathfn_compar_double();

    return mathfn_compar_double(&((know_constant_c*)x)->val,
                                &((know_constant_c*)y)->val);
}

ARRAYTYPED_GENERATE(know_constant_c, 2, 0)
ARRAYTYPED_GENERATE_ORDERED_INSERT(know_constant_c, 1, know_constant_compar_math_constant)

#endif
