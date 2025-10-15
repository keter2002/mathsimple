/*
    integral_aprox - v3.1.0
    Computes a definite integral by right, left, middle, trapezoid and Simpson
    methods.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v3.1.0  Printing precision argument
        v3.0.4  Return 0 in main if help arg
        v3.0.3  Remove torfnum.h
        v3.0.2  atof() replaces torfnum_atof()
        v3.0.1  torfnum_atof macro
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
#include "args.h"


expression_expr expr;
avltree_node *varx;

main(argc,argv)
char *argv[];
{
    void infix_posfix();
    double right_integral(), left_integral(), middle_integral(), trapezoid(), simpson();
    double a,b,n,inc,ri,li;

    avltree_tree controled;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    char *args[] = {"a", "b", "n", "expression"};
    int opt,i;
    int arg_precision = 6;

    for (; (opt = getopt_long(argc, argv, "p:", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case '?':
            fputs("Try 'integral_aprox --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: integral_aprox [OPTION] a b n expression [a=5] [...]\n"
                  "Computes a definite integral by right, left, middle, trapezoid and Simpson\n"
                  "methods.\n\n"
                  "Inform the function to integrate in 'expression', functions with only one\n"
                  "variable and without the derivative of it are supported.\n"
                  "The upper and lower boundaries of the integral are 'a' and 'b'.\n"
                  "'n' is the number of iterations to approximate the result.\n"
                  "x is the variable with respect to the derivative.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n",
                  stdout);
            return 0;
        }

    args_need_all_args(args);

    a = atof(argv[optind]); b = atof(argv[optind+1]); n = atof(argv[optind+2]);

    expression_infix_posfix(&expr, argv[optind+3]);
    if (!(varx = avltree_find_node(expr.vars, "x"))) {
        fputs("variable x not found.\n", stderr);
        expression_destroy(expr);
        return EXIT_FAILURE;
    }
    avltree_create(controled, 1, strcmp, NULL, NULL);
    avltree_insert_key(controled, "x");
    read_vars(&expr, &controled, argc - optind - args_arg_len(args),
              &argv[optind + args_arg_len(args)]);

    expression_show_expr(stdout, &expr);
    inc = (b-a)/n;
    printf("%2$.*1$lf %3$.*1$lf %4$.*1$lf %5$.*1$lf\n",
           arg_precision, a, b, n, inc);

    *(double*)varx->value = b;
    ri = expression_evaluate(&expr.exp);
    *(double*)varx->value = a;
    li = expression_evaluate(&expr.exp);
    printf("RI - LI = %.*lf\n", arg_precision, inc*(ri-li));

    printf("RI: %.*lf\n", arg_precision, right_integral(a, b, n));
    printf("LI: %.*lf\n", arg_precision, left_integral(a, b, n));
    printf("MI: %.*lf\n", arg_precision, middle_integral(a, b, n));
    printf("Trapezoidal Rule: %.*lf\n", arg_precision, trapezoid(a,b,n));
    if (!((int)n%2))
        printf("Simpson Rule: %.*lf\n", arg_precision, simpson(a,b,n));
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
