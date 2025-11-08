/*
    evaluate - v1.0.0
    Evaluates the expression.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.0.0  First version

    evaluate is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>

#include <getopt.h>

#include "expression.h"


main(argc, argv)
char *argv[];
{
    expression_expr expr;
    avltree_tree controled;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"precision", required_argument, NULL, 'p'},
        {"verbose", no_argument, NULL, 'v'},
        { 0 },
    };
    int opt;
    int arg_precision = 6;
    unsigned char arg_verbose = 0;

    for (; (opt = getopt_long(argc, argv, "p:v", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case 'v':
            arg_verbose = 1;
            break;
        case '?':
            fputs("Try 'evaluate --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: evaluate [OPTION] expression [a=5] [...]\n"
                  "Evaluates the expression.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 6\n"
                  "  -v, --verbose      prints the expression in Polish postfix notation\n",
                  stdout);
            return 0;
        }

    if (argc - optind < 1) {
        fputs("Need to inform expression\n", stderr);
        return 2;
    }
    expression_infix_posfix(&expr, argv[optind]);
    avltree_create(controled, 1, strcmp, NULL, NULL);
    read_vars(&expr, &controled, argc - optind - 1,
              &argv[optind + 1]);
    if (arg_verbose)
        expression_show_expr(stdout, &expr, arg_precision);
    printf("%.*f\n", arg_precision, expression_evaluate(&expr.exp));
    expression_destroy(expr);

    return 0;
}
