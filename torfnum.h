/*
    Declarations of functions to convert from number to string or from string
    to number in C.
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

#ifndef H_TORFNUM
#define H_TORFNUM

#include "mathfn.h"


int torfnum_signal(char s[], int *i);
double torfnum_value(char s[], int *i);
void torfnum_reverse(char s[]);
double torfnum_atof(char s[]);
int torfnum_atoi(char s[]);
mathfn_positive_int torfnum_atopi(char s[]);
int torfnum_ftoa(double n, char s[], int lim);
int torfnum_itoa(int n, char s[], int lim);

#endif
