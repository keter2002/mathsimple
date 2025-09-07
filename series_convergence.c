/*
    series_convergence - v1.0.0
    Sum the terms of an infinite summation.
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
#include "expression.h"

main(argc, argv)
char *argv[];
{
    array_dynamic fullexp;
    int i,next;
    double sum;

    if (argc < 2) {
        fputs("Usage: series_convergence expression\n", stderr);
        return 2;
    }
    expression_infix_posfix(&fullexp, argv[1]);
    expression_show_expr(&fullexp);
    for (i=1,sum=0, next=1; i <= 10000000; i++) {
        sum += expression_evaluate(&fullexp, (double)i);
        if (i==next) {
            printf("%.5f [%d]\n", sum, i);
            next *= 10;
        }
    }
}
