/*
    getop.c - v2.0.2
    Parse an operator of a mathematical expression in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.0.2  Catch EOF while reading spaces
        v2.0.1  Uses getc() and ungetc() instead of ch() and unch()
        v2.0.0  Remove torfnum.h and refactor the code
        v1.0.2  Use atof() to replace torfnum_atof() and atoi() to replace
                torfnum_atoi()
        v1.0.1  include torfnum.h
        v1.0.0  First version

    getop.c is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <string.h>
#include <stdlib.h>

#include "mathfn.h"
#include "expression.h"


/*tamanho máximo de um operando ou de um operador*/
#define MAXOP 20
/*signal que um número foi lido*/
#define NUMBER '0'
/*signal que a cadeia está muito grande*/
#define TOOBIG '9'

#define IS_SPACE(C) ((C) == ' ' || (C) == '\t' || (C) == '\n')

getop(s, lim, fp)
char *s;
FILE *fp;
{
    static char exp[MAXOP];

    int c, neg = 0, trunc;
    char *aux = s, *e = exp;
    
    /* Jump spaces. */
    for (c = getc(fp); c != EOF && IS_SPACE(c); c = getc(fp));
    /* If not a number, return. */
    if (!EXPRESSION_IS_DEC_SEP(c) && (c < '0' || c > '9'))
        return c;
    /* s gets the floating point number. */
    for (; c >= '0' && c <= '9'; c = getc(fp))
        if (aux-s < lim)
            *aux++ = c;

    if (EXPRESSION_IS_DEC_SEP(c))
        do {
            if (aux-s < lim)
                *aux++ = c;
            c = getc(fp);
        } while (c >= '0' && c <= '9');

    if (c == 'e' || c == 'E') { /* if have a exponent. */
        c = getc(fp);
        if (c == '-' || c == '+') {
            neg = c == '-';
            c = getc(fp);
        }
        if (c < '0' || c > '9')
            return c;

        do {
            if (e-exp < MAXOP)
                *e++ = c;
            c = getc(fp);
        } while (c >= '0' && c <= '9');
        *aux = *e = '\0';
        
        /* Number needs to fit in lim. */
        if (neg)
            trunc = strfromd(exp, lim, "%f", atof(s) / mathfn_powi(10, atoi(exp)));
        else
            trunc = strfromd(exp, lim, "%f", atof(s) * mathfn_powi(10, atoi(exp)));
        if (!trunc) {
            strcpy(s, exp);
            return NUMBER;
        }
    } else if (aux-s < lim) { /* Number fits. */
        ungetc(c, fp);
        *aux = '\0';
        return NUMBER;
    }
    
    /* Number too big or with error */
    while (!IS_SPACE(c) && c != EOF)
        c = getc(fp);
    *aux = '\0';
    return TOOBIG;
}
