/*
    print_series - v2.1.0
    Prints the terms of a sequence.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.1.0  Printing precision argument
        v2.0.3  atoi() replaces torfnum_atoi()
        v2.0.2  Change order of parameters in read_vars
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

#include <getopt.h>

#include "expression.h"
#include "args.h"


main(argc, argv)
char *argv[];
{
    expression_expr expr;
    avltree_tree controled;
    avltree_node *x;
    int i, start, end, n;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    char *args[] = {"start", "n", "expression"};
    int opt;
    int arg_precision = 5;

    for (; (opt = getopt_long(argc, argv, "p:", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case '?':
            fputs("Try 'print_series --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: print_series [OPTION] start n expression [a=5] [...]\n"
                  "Prints the terms of a sequence.\n\n"
                  "The variable x is special here, it starts with start and is incremented n times\n"
                  "to print the terms.\n\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 5\n",
                  stdout);
            return 0;
        }
    
    args_need_all_args(args);

    expression_infix_posfix(&expr, argv[optind+2]);
    if (!(x = avltree_find_node(expr.vars, "x"))) {
        fputs("variable x not found.\n", stderr);
        expression_destroy(expr);
        return EXIT_FAILURE;
    }
    avltree_create(controled, 1, strcmp, NULL, NULL);
    avltree_insert_key(controled, "x");
    read_vars(&expr, &controled, argc - optind - args_arg_len(args),
              &argv[optind + args_arg_len(args)]);
    i = start = atoi(argv[optind]);
    n = atoi(argv[optind+1]);

    expression_show_expr(stdout, &expr);
    for (end=start+n; i < end; i++) {
        *(double*)x->value = i;
        printf("%.*f%c", arg_precision, expression_evaluate(&expr.exp),
               i < end-1?  ' ' : '\n');
    }
    expression_destroy(expr);

    return 0;
}
