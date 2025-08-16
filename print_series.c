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

#include "expression.c"
#include <stdio.h>

main(argc, argv)
char *argv[];
{
    extern int atoi_torfnum();
    dynamic_array fullexp;
    int i, start, n;

    if (argc < 4) {
        fputs("Usage: print_series start n expression\n", stderr);
        return 2;
    }
    i = start = atoi_torfnum(argv[1]);
    n = atoi_torfnum(argv[2]);
    infix_posfix(&fullexp, argv[3]);
    show_expr(&fullexp);
    for (; i < start+n; i++)
        printf(i == start+n-1? "%.5f\n" : "%.5f ", evaluate(&fullexp, (double)i));
}
