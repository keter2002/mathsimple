/*
    Definitions of functions to convert from number to string or from string to
    number in C.
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
#include "torfnum.h"


int signal_torfnum(s, i)
char s[];
int *i;
{
    if (s[*i] == '+' || s[*i] == '-')
        return (s[(*i)++]=='+') ? 1 : -1;
    return 1;
}

double value_torfnum(s, i)
char s[];
int *i;
{
    double val;

    for (val = 0; s[*i] >= '0' && s[*i] <= '9'; ++*i)
        val = 10 * val + s[*i] - '0';
    return val;
}

positive_int_mathfn valuei_torfnum(s, i)
char s[];
int *i;
{
    positive_int_mathfn val;

    for (val = 0; s[*i] >= '0' && s[*i] <= '9'; ++*i)
        val = 10 * val + s[*i] - '0';
    return val;
}

double atof_torfnum(s)
char s[];
{
    double val, power;
    int i, sign, ex;

    for (i=0; s[i]==' ' || s[i]=='\n' || s[i]=='\t'; i++);
    sign = signal_torfnum(s, &i);
    val = value_torfnum(s, &i);
    if (s[i] == '.' || s[i] == ',')
        i++;
    for (power = 1; s[i] >= '0' && s[i] <= '9'; i++) {
        val = 10 * val + s[i] - '0';
        power *= 10;
    }
    
    val /= power * sign;
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        sign = signal_torfnum(s, &i);
        ex = value_torfnum(s, &i);
        val = (sign==1) ? val * powi_mathfn(10, ex) : val / powi_mathfn(10, ex);
    }
    return(val);
}

atoi_torfnum(s)
char s[];
{
    int val;
    int i, sign, ex;

    for (i=0; s[i]==' ' || s[i]=='\n' || s[i]=='\t'; i++);
    sign = signal_torfnum(s, &i);
    val = sign * value_torfnum(s, &i);
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        sign = signal_torfnum(s, &i);
        ex = value_torfnum(s, &i);
        val = (sign==1) ? val * powi_mathfn(10, ex) : val / powi_mathfn(10, ex);
    }
    return val;
}

positive_int_mathfn atopi_torfnum(s)
char s[];
{
    positive_int_mathfn val, ex;
    int i, sign;

    for (i=0; s[i]==' ' || s[i]=='\n' || s[i]=='\t'; i++);
    sign = signal_torfnum(s, &i);
    val = sign * valuei_torfnum(s, &i);
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        sign = signal_torfnum(s, &i);
        ex = valuei_torfnum(s, &i);
        val = (sign==1) ? val * powi_mathfn(10, ex) : val / powi_mathfn(10, ex);
    }
    return val;
}

ftoa_torfnum(n, s, lim)
double n;
char s[];
{
    int nI, i;
    
    nI = (int)n;
    if (itoa_torfnum(nI, s, lim))
        return(-1);
    
    for (i = 0; s[i] != '\0'; i++);
    
    if (i > lim)
        return (-1);
    s[i++] = '.';
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

void reverse_torfnum(s)
char *s;
{
    char c;
    char *end;

    for (end = s+strlen(s)-1; s < end; s++, end--){
        c = *s;
        *s = *end;
        *end = c;
    }    
}

itoa_torfnum(n, s, l)
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
        return (-1);
    /*preenche com espaços em branco, se não obteve o tamanho mínimo*/
    for (i = s-aux; i < l; *s++ = ' ', i++);
    
    *s = '\0';
    reverse_torfnum(aux);
}
