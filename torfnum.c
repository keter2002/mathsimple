/*
    torfnum.c - v2.0.0
    Definitions of functions to convert from number to string or from string to
    number in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>
    
    History:
        v2.0.0  Remove torfnum_value(), torfnum_strtod(), torfnum_atoi()
                and torfnum_reverse()
        v1.1.0  torfnum_strtod()
        v1.0.1  Return 0 on success of torfnum_itoa()
        v1.0.0  First version

    torfnum.c is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include "torfnum.h"


int torfnum_signal(s, i)
char s[];
int *i;
{
    if (s[*i] == '+' || s[*i] == '-')
        return (s[(*i)++]=='+') ? 1 : -1;
    return 1;
}

mathfn_positive_int valuei_torfnum(s, i)
char s[];
int *i;
{
    mathfn_positive_int val;

    for (val = 0; s[*i] >= '0' && s[*i] <= '9'; ++*i)
        val = 10 * val + s[*i] - '0';
    return val;
}

mathfn_positive_int torfnum_atopi(s)
char s[];
{
    mathfn_positive_int val, ex;
    int i, sign;

    for (i=0; s[i]==' ' || s[i]=='\n' || s[i]=='\t'; i++);
    sign = torfnum_signal(s, &i);
    val = sign * valuei_torfnum(s, &i);
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        sign = torfnum_signal(s, &i);
        ex = valuei_torfnum(s, &i);
        val = (sign==1) ? val * mathfn_powi(10, ex) : val / mathfn_powi(10, ex);
    }
    return val;
}

torfnum_ftoa(n, s, lim)
double n;
char s[];
{
    int nI, i;
    
    nI = (int)n;
    if (torfnum_itoa(nI, s, lim))
        return(-1);
    
    for (i = 0; s[i] != '\0'; i++);
    
    if (i > lim)
        return (-1);
    s[i++] = TORFNUM_DEC_SEP;
    if (i > lim)
        return (-1);

    do {
        n = (n - nI) * 10;
        nI = (int)n;
        s[i++] = '0' + n;
    } while(n != 0.0 && i < lim);

    s[i] = '\0';
    return(0);
}

torfnum_itoa(n, s, l)
char *s;
{
    int i, sign;
    char *aux = s;

    if ((sign = n) > 0)
        n = -n; 
    /*gera os digitos invertidos*/
    do {
        /*obtem o proximo digito*/
        *s++ = '0' - n % 10;
        if (s-aux > l)
            return (-1);
    /*remove o digito*/
    } while ((n /= 10) < 0);
    if (sign < 0)
        *s++ = '-';

    if (s-aux > l)
        return -1;
    /*preenche com espaços em branco, se não obteve o tamanho mínimo*/
    for (i = s-aux; i < l; *s++ = ' ', i++);
    
    *s = '\0';
    reverse(aux);
    return 0;
}
