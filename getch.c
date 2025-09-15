/*
    getch.c - v1.0.0
    ch and unch functions in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    getch.c is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>

#define TRUE 1
#define FALSE 0

static cha;
static entry = FALSE;

ch(fp)
FILE *fp;
{
    if (entry) {
        entry = FALSE;
        return cha;
    }
    return getc(fp);
}

void unch(c)
{
    if (entry)
        fputs("unch: too much characters", stderr);
    else {
        cha = c;
        entry = TRUE;
    }
}
