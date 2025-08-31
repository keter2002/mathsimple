/*
    Computes a definite integral by right, left, middle, trapezoid and Simpson
    methods.
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
#include "expression.c"
#include <getopt.h>

array_dynamic fullexp;

main(argc,argv)
char *argv[];
{
    extern double torfnum_atof();
    void infix_posfix();
    double right_integral(), left_integral(), middle_integral(), trapezoid(), simpson();
    double a,b,n;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        { 0 },
    };
    int opt;
    
    opt = getopt_long(argc, argv, "h", long_opts, NULL);
    if (argc < 5 || opt == 'h' || opt == '?') {
        fputs("Usage: expression a b n\n"
              "Computes a definite integral by right, left, middle, trapezoid and Simpson\n"
              "methods.\n\n"
              "Inform the function to integrate in 'expression', functions with only one\n"
              "variable and without the derivative of it are supported.\n"
              "The upper and lower boundaries of the integral are 'a' and 'b'.\n"
              "'n' is the number of iterations to approximate the result.\n",
              stderr);
        return 2;
    }

    infix_posfix(&fullexp, argv[1]);
    show_expr(&fullexp);
    a = torfnum_atof(argv[2]); b = torfnum_atof(argv[3]); n = torfnum_atof(argv[4]);
    printf("RI: %lf\n", right_integral(a, b, n));
    printf("LI: %lf\n", left_integral(a, b, n));
    printf("MI: %lf\n", middle_integral(a, b, n));
    printf("Trapezoidal Rule: %lf\n", trapezoid(a,b,n));
    if (!((int)n%2))
        printf("Simpson Rule: %lf\n", simpson(a,b,n));
}

double right_integral(a, b, n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    printf("%lf %lf %lf %lf\n", a, b, n, inc);
    printf("RI - LI = %lf\n", inc*(evaluate(&fullexp, b)-evaluate(&fullexp, a)));
    res = 0;
    for (x=a+inc,i=0; i < n; x+=inc, i++)
        res += evaluate(&fullexp, x);
    return res * inc;
}

double left_integral(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    res = 0;
    for (x=a,i=0; i < n; x+=inc, i++)
        res += evaluate(&fullexp, x);
    return res * inc;
}

double middle_integral(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    res = 0;
    for (x=a+inc/2.0f,i=0; i < n; x+=inc,i++)
        res += evaluate(&fullexp, x);
    return res * inc;
}

double trapezoid(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    x=a;
    res = evaluate(&fullexp, x)/2.0;
    for (x+=inc, i=1; i < n; x+=inc,i++)
        res += evaluate(&fullexp, x);
    res += evaluate(&fullexp, x)/2.0;
    return res * inc;
}

double simpson(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    x=a;
    res = evaluate(&fullexp, x);
    for (x+=inc, i=1; i < n; x+=inc,i++)
        res += i%2? 4*evaluate(&fullexp, x) : 2*evaluate(&fullexp, x);
    res += evaluate(&fullexp, x);
    return res * inc / 3.0;
}
