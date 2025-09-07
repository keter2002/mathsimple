/*
    expression.c - v1.0.0
    Mathematical expression parser definitions.
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
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#include "expression.h"

#define HASH(x) (x - '*')

static char op['^'-'*'] = { 0 };

#define PRIORITIES \
     op[HASH('+')]=1; \
     op[HASH('-')]=1; \
     op[HASH('*')]=2; \
     op[HASH('/')]=2; \
     op[HASH('^')]=3;

void expression_insert(fullexp, op, f, fn, type)
array_dynamic *fullexp;
char op;
double f;
double (*fn)();
{
    switch (type) {
    case EXPRESSION_OP_TYPE_OP:
        ((expression_op*)ARRAY_LAST_SPACE_PTR(fullexp))->symb.opval = op;
        break;
    case EXPRESSION_OP_TYPE_F:
        ((expression_op*)ARRAY_LAST_SPACE_PTR(fullexp))->symb.fval = f;
        break;
    case EXPRESSION_OP_TYPE_FN:
        ((expression_op*)ARRAY_LAST_SPACE_PTR(fullexp))->symb.fnval = fn;
        break;
    }
    ((expression_op*)ARRAY_LAST_SPACE_PTR(fullexp))->utype = type;
    fullexp->nmemb++;
    array_expand_ptr(fullexp,1,5);
}

void expression_infix_posfix(fullexp, c)
array_dynamic *fullexp;
char *c;
{
    extern char *strncpy_while_type();
    array_dynamic stack;
    expression_op *ptrop;
    #define MAX_NUMERIC_PRECISION 10
    char buffer[MAX_NUMERIC_PRECISION + 1];
    int endb;
    double (*fn)();

    endb = 0;
    PRIORITIES
    array_allocate_ptr(fullexp, sizeof(expression_op), 5);
    array_allocate(stack, sizeof(expression_op), 16);
    for (; *c; c++) {
        /* Read a function. */
        if (*c == '\\') {
            strncpy_while_type(buffer, c+1, MAX_NUMERIC_PRECISION, isalnum);
            if (!strcmp(buffer, "cos")) {
                c+=4;
                fn = cos;
            } else if (!strcmp(buffer, "sin")) {
                c+=4;
                fn = sin;
            } else if (!strcmp(buffer, "tan")) {
                c+=4;
                fn = tan;
            } else if (!strcmp(buffer, "ln")) {
                c+=3;
                fn = log;
            } else {
                fprintf(stderr, "[%s:%s] function not supported (%s).\n", __FILE__, __func__, buffer);
                exit(EXIT_FAILURE);
            }
            expression_insert(&stack, '(', 0, 0, EXPRESSION_OP_TYPE_OP);
            expression_insert(&stack, 0, 0, fn, EXPRESSION_OP_TYPE_FN);
        /* Read a number or a variable. */
        } else if (isalnum(*c) || *c == '.') {
            if (isdigit(*c) || *c == '.') {
                assert(endb < MAX_NUMERIC_PRECISION);
                buffer[endb++] = *c;
            } else
                expression_insert(fullexp, *c, 0, 0, EXPRESSION_OP_TYPE_OP);
        /* Read a operator. */
        } else if (*c == '+' || *c == '-' || *c == '*' || *c == '/' || *c == '^') {
            /* If has a number before, place it. */
            if (endb){
                buffer[endb]='\0';
                expression_insert(fullexp, 0, atof(buffer), 0, EXPRESSION_OP_TYPE_F);
                endb=0;
            }
            /* Operates first with more priority. */
            ptrop = ARRAY_AT(stack, stack.nmemb-1);
            while (stack.nmemb && ptrop->utype == EXPRESSION_OP_TYPE_OP &&
                   op[HASH(*c)] <= op[HASH(ptrop->symb.opval)]) {
                expression_insert(fullexp, ptrop->symb.opval, 0, 0,
                                  EXPRESSION_OP_TYPE_OP);
                stack.nmemb--;
                ptrop = ARRAY_AT(stack, stack.nmemb-1);
            }
            expression_insert(&stack, *c, 0, 0, EXPRESSION_OP_TYPE_OP);
        } else if (*c == '(')
            expression_insert(&stack, *c, 0, 0, EXPRESSION_OP_TYPE_OP);
        else if (*c == ')') {
            if (endb){
                buffer[endb]='\0';
                expression_insert(fullexp, 0, atof(buffer), 0,
                                  EXPRESSION_OP_TYPE_F);
                endb=0;
            }
            /* Put all operators inside the last parenthesis. */
            ptrop = ARRAY_AT(stack, stack.nmemb-1);
            while (stack.nmemb && ((ptrop->utype == EXPRESSION_OP_TYPE_OP &&
                                    ptrop->symb.opval != '(') ||
                                   ptrop->utype == EXPRESSION_OP_TYPE_FN)) {
                expression_insert(fullexp, ptrop->symb.opval, 0,
                                  ptrop->symb.fnval, ptrop->utype);
                stack.nmemb--;
                ptrop = ARRAY_AT(stack, stack.nmemb-1);
            }
            stack.nmemb--;
        }
    }
    /* If last was a number. */
    if (endb) {
        buffer[endb]='\0';
        expression_insert(fullexp, 0, atof(buffer), 0, EXPRESSION_OP_TYPE_F);
        endb=0;
    }
    /* Puts all remaining operators. */
    ptrop = ARRAY_AT(stack, stack.nmemb-1);
    while (stack.nmemb && ptrop->utype == EXPRESSION_OP_TYPE_OP) {
        expression_insert(fullexp, ptrop->symb.opval, 0, 0,
                          EXPRESSION_OP_TYPE_OP);
        stack.nmemb--;
        ptrop = ARRAY_AT(stack, stack.nmemb-1);
    }
    free(stack.base);
}

static array_dynamic stack = { 0 };

double expression_evaluate(fullexp, x)
array_dynamic *fullexp;
double x;
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
        case EXPRESSION_OP_TYPE_F:
            *(double*)ARRAY_LAST_SPACE(stack) = next->symb.fval;
            stack.nmemb++;
            array_expand(stack,1,5);
            break;
        case EXPRESSION_OP_TYPE_FN:
            op1 = ARRAY_AT(stack, stack.nmemb-1);
            *op1 = next->symb.fnval(*op1);
            stack.nmemb--;
            break;
        default:
            if (next->symb.opval == 'x') {
                *(double*)ARRAY_LAST_SPACE(stack) = x;
                stack.nmemb++;
                array_expand(stack,1,5);
            } else {
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
            }
            break;
        }
    }
    res = *(double*)stack.base;
    return res;
}

void expression_show_expr(fullexp)
array_dynamic *fullexp;
{
    expression_op *next;
    double (*fn)();
    int i;

    for (i=0; i < fullexp->nmemb; i++) {
        next = ARRAY_AT_PTR(fullexp, i);
        switch (next->utype) {
        case EXPRESSION_OP_TYPE_OP:
            putchar(next->symb.opval);
            break;
        case EXPRESSION_OP_TYPE_F:
            printf("|%lf|", next->symb.fval);
            break;
        case EXPRESSION_OP_TYPE_FN:
            fn = next->symb.fnval;
            if (fn == cos) {
                printf("|cos|");
            } else if (fn == sin) {
                printf("|sin|");
            } else if (fn == tan) {
                printf("|tan|");
            } else if (fn == log) {
                printf("|log|");
            }
        }
    }
    putchar('\n');
}
