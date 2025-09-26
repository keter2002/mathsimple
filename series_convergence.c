/*
    series_convergence - v2.0.1
    Sum the terms of an infinite summation.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.0.2  Change order of parameters in read_vars
        v2.0.1  Checks missing variables in expression
        v2.0.0  Changes in expression syntax and support to variables
        v1.0.0  First version

    series_convergence is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "expression.h"

main(argc, argv)
char *argv[];
{
    expression_expr expr;
    avltree_tree controled;
    avltree_node *x;
    int i,next;
    double sum;

    if (argc < 2) {
        fputs("Usage: series_convergence expression [a=5] [...]\n"
              "Sum the terms of an infinite summation.\n\n"
              "The variable x is special here, it starts with value one and is incremented to\n"
              "make the test of convergence.\n", stderr);
        return 2;
    }
    expression_infix_posfix(&expr, argv[1]);
    if (!(x = avltree_find_node(expr.vars, "x"))) {
        fputs("variable x not found.\n", stderr);
        expression_destroy(expr);
        return EXIT_FAILURE;
    }
    avltree_create(controled, 1, strcmp, NULL, NULL);
    avltree_insert_key(controled, "x");
    read_vars(&expr, &controled, argc-2, &argv[2]);

    expression_show_expr(stdout, &expr);
    for (i=1,sum=0, next=1; i <= 10000000; i++) {
        *(double*)x->value = i;
        sum += expression_evaluate(&expr.exp);
        if (i==next) {
            printf("%.5f [%d]\n", sum, i);
            next *= 10;
        }
    }
    expression_destroy(expr);
}
