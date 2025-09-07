/*
    factorizer - v1.0.0
    Prints the factors of an integer number.
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
#include <stdlib.h>

main(argc, argv)
char *argv[];
{
    extern torfnum_atoi();
    int num, factor;

    if (argc < 2) {
        puts("Usage: factorizer x");
        return 2;
    }
    num = torfnum_atoi(argv[1]);
    factor = 2;
    puts("1");
    while (num != 1)
        if (!(num % factor)) {
            printf("%d\n", factor);
            num /= factor;
            factor = 2;
        } else
            factor++;
}
