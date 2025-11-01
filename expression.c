/*
    expression.c - v5.0.0
    Mathematical expression parser definitions.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v5.0.0  Add other common logarithm functions and change output string
                of natural logarithm
        v4.2.6  fix a bug when calling functions in expression_evaluate()
        v4.2.5  strsave() instead of strdup()
        v4.2.4  EXPRESSION_IS_DEC_SEP
        v4.2.3  Use atof() to replace torfnum_atof() and strtod() to
                replace torfnum_strtod()
        v4.2.2  Simplification of expression_infix_posfix() parsing and
                torfnum_atof macro
        v4.2.1  Pi has to jump one more char
        v4.2.0  Unary '-' and '+'
        v4.1.0  Euler's number and pi
        v4.0.0  Change order of parameters in read_vars
        v3.0.0  Checks missing variables in expression
        v2.0.0  Changes in expression syntax and support to variables
        v1.0.0  First version

    expression.c is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#include "external/avltree/avltree.c"

#include "expression.h"

#define HASH(x) (x - '*')

static char op['^'-'*'] = { 0 };

#define PRIORITIES \
     op[HASH('+')]=1; \
     op[HASH('-')]=1; \
     op[HASH('*')]=2; \
     op[HASH('/')]=2; \
     op[HASH('^')]=3;

void expression_insert(expr, op, f, fn, name, type)
expression_expr *expr;
char op;
double f;
double (*fn)();
char name[];
{
    extern char *strsave();
    avltree_node *saved;
    double *new;
    char *newname;

    switch (type) {
    case EXPRESSION_OP_TYPE_OP:
        ((expression_op*)ARRAY_LAST_SPACE(expr->exp))->symb.opval = op;
        break;
    case EXPRESSION_OP_TYPE_F:
        ((expression_op*)ARRAY_LAST_SPACE(expr->exp))->symb.fval = f;
        break;
    case EXPRESSION_OP_TYPE_FN:
        ((expression_op*)ARRAY_LAST_SPACE(expr->exp))->symb.fnval = fn;
        break;
    case EXPRESSION_OP_TYPE_NAME:
        /* If first time that name is seen, associates a space with it.
         * Else return this space. */
        if ((saved = avltree_find_node(expr->vars, name))) {
            new = saved->value;
        } else {
            new = malloc(sizeof(double));
            newname = strsave(name);
            avltree_insert(&expr->vars, newname, new);
            avltree_insert(&expr->vars_rev, new, newname);
        }
        ((expression_op*)ARRAY_LAST_SPACE(expr->exp))->symb.nameval = new;
        break;
    }
    ((expression_op*)ARRAY_LAST_SPACE(expr->exp))->utype = type;
    expr->exp.nmemb++;
    array_expand(expr->exp,1,5);
}

compar_ptr(x, y)
void *x, *y;
{
    return (uintptr_t)x-(uintptr_t)y;
}

#define IS_OP(C) \
    ((C) == '+' || (C) == '-' || (C) == '*' || (C) == '/' || (C) == '^')

void expression_infix_posfix(expr, str)
expression_expr *expr;
char *str;
{
    extern void strncpy_while_type();
    expression_expr stack;
    expression_op *ptrop;
    char buffer[EXPRESSION_MAX_NAME_LEN + 1];
    char *ptr, *endptr;
    double (*fn)();

    PRIORITIES
    array_allocate(expr->exp, sizeof(expression_op), 5);
    array_allocate(stack.exp, sizeof(expression_op), 16);
    avltree_create(expr->vars, 1, strcmp, NULL, NULL);
    avltree_create(expr->vars_rev, 1, compar_ptr, NULL, NULL);
    for (ptr=str; *ptr; ptr++) {
        if (isdigit(*ptr) || EXPRESSION_IS_DEC_SEP(*ptr)) { /* Read a number. */
            expression_insert(expr, 0, strtod(ptr, &endptr), 0, 0, EXPRESSION_OP_TYPE_F);
            ptr = endptr-1;
        } else if (isalpha(*ptr)) { /* Read a variable or a function. */
            strncpy_while_type(buffer, ptr, EXPRESSION_MAX_NAME_LEN, isalnum);

            if (!strcmp(buffer, "e")) { /* Euler's number. */
                expression_insert(expr, 0, M_E, 0, 0, EXPRESSION_OP_TYPE_F);
                continue;
            } else if (!strcmp(buffer, "pi")) {
                ptr++;
                expression_insert(expr, 0, M_PI, 0, 0, EXPRESSION_OP_TYPE_F);
                continue;
            }

            if (strlen(buffer) == 1)
                goto read_var;

            if (!strcmp(buffer, "cos")) {
                ptr+=3;
                fn = cos;
            } else if (!strcmp(buffer, "sin")) {
                ptr+=3;
                fn = sin;
            } else if (!strcmp(buffer, "tan")) {
                ptr+=3;
                fn = tan;
            } else if (!strcmp(buffer, "ln")) {
                ptr+=2;
                fn = log;
            } else if (!strcmp(buffer, "log")) {
                ptr+=3;
                fn = log10;
            } else if (!strcmp(buffer, "log10")) {
                ptr+=5;
                fn = log10;
            } else if (!strcmp(buffer, "log2")) {
                ptr+=4;
                fn = log2;
            } else { /* Read a variable. */
read_var:
                expression_insert(expr, 0, 0, 0, buffer,
                                  EXPRESSION_OP_TYPE_NAME);
                continue;
            }
            /* It was a function. */
            expression_insert(&stack, '(', 0, 0, 0, EXPRESSION_OP_TYPE_OP);
            expression_insert(&stack, 0, 0, fn, 0, EXPRESSION_OP_TYPE_FN);
        } else if (IS_OP(*ptr)) { /* Read a operator. */
            /* Insert '0' when sees a unary '-' or '+': */
            if ((*ptr == '+' || *ptr == '-') && (ptr == str || ptr[-1] == '(' || IS_OP(ptr[-1]))) {
                expression_insert(expr, 0, 0, 0, 0, EXPRESSION_OP_TYPE_F);
                expression_insert(&stack, *ptr, 0, 0, 0, EXPRESSION_OP_TYPE_OP);
                continue;
            }

            /* Operates first with more priority. */
            ptrop = ARRAY_AT(stack.exp, stack.exp.nmemb-1);
            while (stack.exp.nmemb && ptrop->utype == EXPRESSION_OP_TYPE_OP &&
                   op[HASH(*ptr)] <= op[HASH(ptrop->symb.opval)]) {
                expression_insert(expr, ptrop->symb.opval, 0, 0, 0,
                                  EXPRESSION_OP_TYPE_OP);
                stack.exp.nmemb--;
                ptrop = ARRAY_AT(stack.exp, stack.exp.nmemb-1);
            }
            expression_insert(&stack, *ptr, 0, 0, 0, EXPRESSION_OP_TYPE_OP);
        } else if (*ptr == '(')
            expression_insert(&stack, *ptr, 0, 0, 0, EXPRESSION_OP_TYPE_OP);
        else if (*ptr == ')') {
            /* Put all operators inside the last parenthesis. */
            ptrop = ARRAY_AT(stack.exp, stack.exp.nmemb-1);
            while (stack.exp.nmemb && ((ptrop->utype == EXPRESSION_OP_TYPE_OP &&
                                    ptrop->symb.opval != '(') ||
                                   ptrop->utype == EXPRESSION_OP_TYPE_FN)) {
                expression_insert(expr, ptrop->symb.opval, 0,
                                  ptrop->symb.fnval, 0, ptrop->utype);
                stack.exp.nmemb--;
                ptrop = ARRAY_AT(stack.exp, stack.exp.nmemb-1);
            }
            stack.exp.nmemb--;
        }
    }
    /* Puts all remaining operators. */
    ptrop = ARRAY_AT(stack.exp, stack.exp.nmemb-1);
    while (stack.exp.nmemb && ptrop->utype == EXPRESSION_OP_TYPE_OP) {
        expression_insert(expr, ptrop->symb.opval, 0, 0, 0,
                          EXPRESSION_OP_TYPE_OP);
        stack.exp.nmemb--;
        ptrop = ARRAY_AT(stack.exp, stack.exp.nmemb-1);
    }
    free(stack.exp.base);
}

static array_dynamic stack = { 0 };

double expression_evaluate(fullexp)
array_dynamic *fullexp;
{
    expression_op *next;
    double *op1, *op2, res;
    int i;
    
    if (!stack.capacity)
        array_allocate(stack, sizeof(double), 10);
    stack.nmemb=0;
    for (i=0; i < fullexp->nmemb; i++) {
        next = ARRAY_AT_PTR(fullexp, i);
        switch (next->utype) {
        case EXPRESSION_OP_TYPE_OP:
                op1 = ARRAY_AT(stack, stack.nmemb-2);
                op2 = ARRAY_AT(stack, stack.nmemb-1);
                switch (next->symb.opval) {
                case '+':
                    *op1 += *op2;
                    break;
                case '-':
                    *op1 -= *op2;
                    break;
                case '*':
                    *op1 *= *op2;
                    break;
                case '/':
                    *op1 /= *op2;
                    break;
                case '^':
                    *op1 = pow(*op1, *op2);
                    break;
                }
                stack.nmemb--;
            break;
        case EXPRESSION_OP_TYPE_F:
            *(double*)ARRAY_LAST_SPACE(stack) = next->symb.fval;
            stack.nmemb++;
            array_expand(stack,1,5);
            break;
        case EXPRESSION_OP_TYPE_FN:
            op1 = ARRAY_AT(stack, stack.nmemb-1);
            *op1 = next->symb.fnval(*op1);
            break;
        case EXPRESSION_OP_TYPE_NAME:
            *(double*)ARRAY_LAST_SPACE(stack) = *next->symb.nameval;
            stack.nmemb++;
            array_expand(stack,1,5);
            break;
        }
    }
    res = *(double*)stack.base;
    return res;
}

void expression_show_expr(stream, expr)
FILE *stream;
expression_expr *expr;
{
    expression_op *next;
    avltree_node *name;
    double (*fn)();
    int i;

    for (i=0; i < expr->exp.nmemb; i++) {
        next = ARRAY_AT(expr->exp, i);
        switch (next->utype) {
        case EXPRESSION_OP_TYPE_OP:
            putc(next->symb.opval, stream);
            break;
        case EXPRESSION_OP_TYPE_F:
            fprintf(stream, "|%lf|", next->symb.fval);
            break;
        case EXPRESSION_OP_TYPE_FN:
            fn = next->symb.fnval;
            if (fn == cos) {
                fprintf(stream, "|cos|");
            } else if (fn == sin) {
                fprintf(stream, "|sin|");
            } else if (fn == tan) {
                fprintf(stream, "|tan|");
            } else if (fn == log) {
                fprintf(stream, "|ln|");
            } else if (fn == log10) {
                fprintf(stream, "|log|");
            } else if (fn == log2) {
                fprintf(stream, "|log2|");
            } else {
                fprintf(stream, "|FN|");
            }
            break;
        case EXPRESSION_OP_TYPE_NAME:
            name = avltree_find_node(expr->vars_rev, next->symb.nameval);
            fprintf(stream, "%s", (char*)name->value);
            break;
        }
    }
    putc('\n', stream);
}

void print_key(stream, key, value)
FILE *stream;
void *key, *value;
{
    fprintf(stream, "%s", (char*)key);
}

void read_vars(expr, controled, argc, argv)
expression_expr *expr;
avltree_tree *controled;
char *argv[];
{
    avltree_tree vars_clone;
    avltree_node *var;
    int i;
    char *saveptr1, *saveptr2;
    char *attr, *varname, *varvalue;

    /* Clones all variables present in expr to check missing ones. */
    avltree_create(vars_clone, 1, strcmp, print_key, ", ");
    avltree_copy_keys(vars_clone, expr->vars);
    avltree_diff_ptr(&vars_clone, controled, AVLTREE_FREE_NONE);
    for (i=0; i < argc; i++)
        for (attr=strtok_r(argv[i], ",", &saveptr1); attr;
             attr=strtok_r(NULL, ",", &saveptr1)) {
            varname = strtok_r(attr, "=", &saveptr2);
            if (!(varvalue = strtok_r(NULL, "=", &saveptr2))) {
                fprintf(stderr, "[%s] equality \"=\" not found.\n", __func__);
                exit(EXIT_FAILURE);
            }
            if ((var = avltree_find_node(expr->vars, varname))) {
                *(double*)var->value = atof(varvalue);
                avltree_remove_node(vars_clone, varname, AVLTREE_FREE_NONE);
            } else
                fprintf(stderr, "[%s] %s not present in expression.\n",
                        __func__, varname);
        }
    if (vars_clone.nmemb) {
        fprintf(stderr, "[%s] missing values for: ", __func__);
        avltree_infix(stderr, vars_clone);
        putc('\n', stderr);
        exit(EXIT_FAILURE);
    }
    avltree_empty(vars_clone);
}
