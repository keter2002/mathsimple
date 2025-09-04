/*
    Auxiliar string standard lib functions.
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

#include <stdlib.h>

/* Copies at most n  */
char *strncpy_while_type(dest, src, n, check)
char *dest, *src;
size_t n;
int (*check)();
{
    size_t i;

    for (i=0; i < n && *src && check((unsigned char)*src); i++)
        dest[i] = *src++;
    if (i < n) dest[i] = '\0';
    return dest;
}
