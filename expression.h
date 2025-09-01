/*
    Mathematical expression parser declarations.
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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "external/arrays/array.c"

typedef struct {
    union uop {
        char opval;
        double fval;
    } symb;
    unsigned char utype;
} expression_op;

void expression_insert(array_dynamic *fullexp, char c, double fc, int type);
void expression_infix_posfix(array_dynamic *fullexp, char *c);
double expression_evaluate(array_dynamic *fullexp, double x);
void expression_show_expr(array_dynamic *fullexp);

#endif
