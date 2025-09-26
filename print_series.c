/*
    print_series - v2.0.1
    Prints the terms of a sequence.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.0.1  Checks missing variables in expression
        v2.0.0  Changes in expression syntax and support to variables
        v1.0.0  First version

    print_series is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>

#include "expression.h"

main(argc, argv)
char *argv[];
{
    extern int torfnum_atoi();
    expression_expr expr;
    avltree_tree controled;
    avltree_node *x;
    int i, start, n;

    if (argc < 4) {
        fputs("Usage: print_series start n expression [a=5] [...]\n"
              "Prints the terms of a sequence.\n\n"
              "The variable x is special here, it starts with start and is incremented n times\n"
              "to print the terms.\n", stderr);
        return 2;
    }
    expression_infix_posfix(&expr, argv[3]);
    if (!(x = avltree_find_node(expr.vars, "x"))) {
        fputs("variable x not found.\n", stderr);
        expression_destroy(expr);
        return EXIT_FAILURE;
    }
    avltree_create(controled, 1, strcmp, NULL, NULL);
    avltree_insert_key(controled, "x");
    read_vars(&expr, argc-4, &argv[4], &controled);
    i = start = torfnum_atoi(argv[1]);
    n = torfnum_atoi(argv[2]);

    expression_show_expr(stdout, &expr);
    for (; i < start+n; i++) {
        *(double*)x->value = i;
        printf(i == start+n-1? "%.5f\n" : "%.5f ",
               expression_evaluate(&expr.exp));
    }
    expression_destroy(expr);
}
