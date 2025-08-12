/*
    Parse an operator of a mathematical expression in C.
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

#include <string.h>
#include "mathfn.h"


/*tamanho máximo de um operando ou de um operador*/
#define MAXOP 20
/*signal que um número foi lido*/
#define NUMBER '0'
/*signal que a cadeia está muito grande*/
#define TOOBIG '9'

extern int ch();
extern void unch();

extern int ftoa_torfnum(), atoi_torfnum();
extern double atof_torfnum();

getop(s, lim, fp)
char *s;
FILE *fp;
{
    int c, neg = 0, err;
    char exp[lim];
    char *aux = s, *e = exp;

    while ((c = ch(fp)) == ' ' || c == '\t' || c == '\n');
    
    if (c != '.' && c != ',' && (c != 'e' || c != 'E') && (c < '0' || c > '9'))
        return (c);
    *aux++ = c;
    while ((c = getc(fp)) >= '0' && c <= '9')
        if (aux-s < lim)
            *aux++ = c;
    if (aux-s == 1 && *s == 'e')
        return (c);
    if (c == '.' || c == ',') {
        if (aux-s < lim)
            *aux++ = c;
        while ((c=getc(fp)) >= '0' && c <= '9')
            if (aux-s < lim)
                *aux++ = c;
    }
    
    if (c == 'e' || c == 'E') {
        c = getc(fp);
        if (c == '-' || c == '+') {
            neg = (c=='-') ? 1 : 0;
            while ((c = getc(fp)) >= '0' && c <= '9')
                   *e++ = c;
        } else
            do
                *e++ = c;
            while ((c = getc(fp)) >= '0' && c <= '9');
        *aux = *e = '\0';
        
        if (neg)
            err = ftoa_torfnum(atof_torfnum(s) / powi_mathfn(10, atoi_torfnum(exp)), exp, lim);
        else
            err = ftoa_torfnum(atof_torfnum(s) * powi_mathfn(10, atoi_torfnum(exp)), exp, lim);
        if (!err) {
            strcpy(s, exp);
            return(NUMBER);
        }
    /* número montado */
    } else if (aux-s < lim) { 
        unch(c);
        *aux = '\0';
        return NUMBER;
    }

    /* muito grande */
    while (c != '\n' && c != EOF)
        c = getc(fp);
    s[lim-1] = '\0';
    return TOOBIG;
}
