/*
    torfnum.h - v1.1.0
    Declarations of functions to convert from number to string or from string
    to number in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.1.0  torfnum_strtod(), torfnum_atof macro and TORFNUM_IS_DEC_SEP and
                TORFNUM_DEC_SEP defines
        v1.0.0  First version

    torfnum.h is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef H_TORFNUM
#define H_TORFNUM

#include "mathfn.h"


int torfnum_signal(char s[], int *i);
double torfnum_value(char s[], int *i);
void torfnum_reverse(char s[]);

#define TORFNUM_IS_DEC_SEP(C) ((C) == '.' || (C) == ',')

double torfnum_strtod(char nptr[], char **endptr);

int torfnum_atoi(char s[]);
mathfn_positive_int torfnum_atopi(char s[]);

#define TORFNUM_DEC_SEP '.'

int torfnum_ftoa(double n, char s[], int lim);

int torfnum_itoa(int n, char s[], int lim);

#define torfnum_atof(S) torfnum_strtod(S, NULL)

#endif
