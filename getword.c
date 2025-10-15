/*
    getword.c - v1.0.0
    getword function definition.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v1.0.0  First version

    getword.c is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LETTER 'a'
#define EOW ','
#define MAXLEN 50

/* Reads a word with only letters to w up to lim chars. */
getword(w, lim, fp)
char *w;
FILE *fp;
{
    int c;

    if (!isalpha(c = *w++ = getc(fp))) {
        *w = '\0';
        return c;
    }
    while (--lim > 0) {
        c = *w++ = getc(fp);
        if (!isalpha(c) && !isdigit(c) && c != ' ') {
            ungetc(c, fp);
            break;
        }
    }
    *(w-1) = '\0';
    return LETTER;
}
