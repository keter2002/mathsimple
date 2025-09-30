/*
    integral_aprox - v3.0.0
    Computes a definite integral by right, left, middle, trapezoid and Simpson
    methods.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v3.0.0  Change order of args
        v2.0.5  Remove 'h' option
        v2.0.4  Move printing to only main
        v2.0.3  Change order of parameters in read_vars
        v2.0.2  Checks missing variables in expression
        v2.0.1  Typo in help
        v2.0.0  Changes in expression syntax and support to variables
        v1.0.0  First version

    integral_aprox is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "expression.h"

expression_expr expr;
avltree_node *varx;

main(argc,argv)
char *argv[];
{
    extern double torfnum_atof();
    void infix_posfix();
    double right_integral(), left_integral(), middle_integral(), trapezoid(), simpson();
    double a,b,n,inc,ri,li;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        { 0 },
    };
    int opt;

    avltree_tree controled;

    opt = getopt_long(argc, argv, "", long_opts, NULL);
    if (argc < 5 || opt == 'h' || opt == '?') {
        fputs("Usage: integral_aprox a b n expression [a=5] [...]\n"
              "Computes a definite integral by right, left, middle, trapezoid and Simpson\n"
              "methods.\n\n"
              "Inform the function to integrate in 'expression', functions with only one\n"
              "variable and without the derivative of it are supported.\n"
              "The upper and lower boundaries of the integral are 'a' and 'b'.\n"
              "'n' is the number of iterations to approximate the result.\n"
              "x is the variable with respect to the derivative.\n",
              stderr);
        return 2;
    }
    a = torfnum_atof(argv[1]); b = torfnum_atof(argv[2]); n = torfnum_atof(argv[3]);

    expression_infix_posfix(&expr, argv[4]);
    if (!(varx = avltree_find_node(expr.vars, "x"))) {
        fputs("variable x not found.\n", stderr);
        expression_destroy(expr);
        return EXIT_FAILURE;
    }
    avltree_create(controled, 1, strcmp, NULL, NULL);
    avltree_insert_key(controled, "x");
    read_vars(&expr, &controled, argc-5, &argv[5]);

    expression_show_expr(stdout, &expr);
    inc = (b-a)/n;
    printf("%lf %lf %lf %lf\n", a, b, n, inc);

    *(double*)varx->value = b;
    ri = expression_evaluate(&expr.exp);
    *(double*)varx->value = a;
    li = expression_evaluate(&expr.exp);
    printf("RI - LI = %lf\n", inc*(ri-li));

    printf("RI: %lf\n", right_integral(a, b, n));
    printf("LI: %lf\n", left_integral(a, b, n));
    printf("MI: %lf\n", middle_integral(a, b, n));
    printf("Trapezoidal Rule: %lf\n", trapezoid(a,b,n));
    if (!((int)n%2))
        printf("Simpson Rule: %lf\n", simpson(a,b,n));
    expression_destroy(expr);
}

double right_integral(a, b, n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    res = 0;
    for (x=a+inc,i=0; i < n; x+=inc, i++) {
        *(double*)varx->value = x;
        res += expression_evaluate(&expr.exp);
    }
    return res * inc;
}

double left_integral(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    res = 0;
    for (x=a,i=0; i < n; x+=inc, i++) {
        *(double*)varx->value = x;
        res += expression_evaluate(&expr.exp);
    }
    return res * inc;
}

double middle_integral(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    res = 0;
    for (x=a+inc/2.0f,i=0; i < n; x+=inc,i++) {
        *(double*)varx->value = x;
        res += expression_evaluate(&expr.exp);
    }
    return res * inc;
}

double trapezoid(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    *(double*)varx->value = x = a;
    res = expression_evaluate(&expr.exp)/2.0;
    for (x+=inc, i=1; i < n; x+=inc,i++) {
        *(double*)varx->value = x;
        res += expression_evaluate(&expr.exp);
    }
    *(double*)varx->value = x;
    res += expression_evaluate(&expr.exp)/2.0;
    return res * inc;
}

double simpson(a,b,n)
double a,b,n;
{
    double inc, res, x;
    int i;

    inc = (b-a)/n;
    *(double*)varx->value = x = a;
    res = expression_evaluate(&expr.exp);
    for (x+=inc, i=1; i < n; x+=inc,i++) {
        *(double*)varx->value = x;
        res += i%2? 4*expression_evaluate(&expr.exp) : 2*expression_evaluate(&expr.exp);
    }
    *(double*)varx->value = x;
    res += expression_evaluate(&expr.exp);
    return res * inc / 3.0;
}
