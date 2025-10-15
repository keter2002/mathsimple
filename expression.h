/*
    expression.h - v4.0.2
    Mathematical expression parser declarations.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v4.0.2  EXPRESSION_IS_DEC_SEP macro
        v4.0.1  EXPRESSION_MAX_NAME_LEN definition
        v4.0.0  Change order of parameters in read_vars
        v3.0.0  Checks missing variables in expression.
        v2.0.0  Changes in expression syntax and support to variables
        v1.0.0  First version

    expression.h is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "external/avltree/avltree.h"
#include "external/arrays/array.c"

typedef struct {
    union uop {
        char opval;
        double fval;
        double (*fnval)();
        double *nameval;
    } symb;
    unsigned char utype;
} expression_op;

#define EXPRESSION_OP_TYPE_OP   0
#define EXPRESSION_OP_TYPE_F    1
#define EXPRESSION_OP_TYPE_FN   2
#define EXPRESSION_OP_TYPE_NAME 3

typedef struct {
    array_dynamic exp;
    avltree_tree vars, vars_rev;
} expression_expr;

/* Caution, expression_infix_posfix must have been called. */
#define expression_destroy_ptr(EXPR)\
    do {\
        avltree_destroy((EXPR)->vars);\
        avltree_empty(&(EXPR)->vars_rev);\
        free((EXPR)->exp.base);\
    } while (0)
#define expression_destroy(EXPR)\
    do {\
        avltree_destroy((EXPR).vars);\
        avltree_empty((EXPR).vars_rev);\
        free((EXPR).exp.base);\
    } while (0)

void expression_insert(expression_expr *expr, char op, double f,
                       double (*fn)(), char name[], int type);

#define EXPRESSION_MAX_NAME_LEN 25
#define EXPRESSION_IS_DEC_SEP(C) ((C) == '.' || (C) == ',')
void expression_infix_posfix(expression_expr *expr, char *str);

double expression_evaluate(array_dynamic *fullexp);

void expression_show_expr(FILE *stream, expression_expr *expr);
void read_vars(expression_expr *expr, avltree_tree *controled, int argc,
               char *argv[]);

#endif
