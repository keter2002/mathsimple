/*
    Prints the terms of a sequence.
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
    extern int torfnum_atoi();
    array_dynamic fullexp;
    int i, start, n;

    if (argc < 4) {
        fputs("Usage: print_series start n expression\n", stderr);
        return 2;
    }
    i = start = torfnum_atoi(argv[1]);
    n = torfnum_atoi(argv[2]);
    expression_infix_posfix(&fullexp, argv[3]);
    expression_show_expr(&fullexp);
    for (; i < start+n; i++)
        printf(i == start+n-1? "%.5f\n" : "%.5f ", expression_evaluate(&fullexp, (double)i));
}
