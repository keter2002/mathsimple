/*
    Mathematical expression parser implementation in C.
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

#include "external/arrays/array.c"
#include <stdio.h>
#include <math.h>
#include <ctype.h>

/*fazer o cosseno...*/

#define HASH(x) (x - '*')

static char op['^'-'*'] = { 0 };

#define PRIORITIES \
     op[HASH('+')]=1; \
     op[HASH('-')]=1; \
     op[HASH('*')]=2; \
     op[HASH('/')]=2; \
     op[HASH('^')]=3;

typedef struct {
    union uop {
        char opval;
        double fval;
    } symb;
    unsigned char utype;
} expression;

void insert(fullexp, c, fc, type)
dynamic_array *fullexp;
char c;
double fc;
{
    if (type)
        ((expression*)LAST_SPACE_PTR_ARRAY(fullexp))->symb.fval = fc;
    else
        ((expression*)LAST_SPACE_PTR_ARRAY(fullexp))->symb.opval = c;
    ((expression*)LAST_SPACE_PTR_ARRAY(fullexp))->utype = type;
    fullexp->nmemb++;
    expand_ptr_array(fullexp,1,5);
}

void infix_posfix(fullexp, c)
dynamic_array *fullexp;
char *c;
{
    dynamic_array stack;
    char buffer[10];
    int endb;
    
    endb = 0;
    PRIORITIES
    allocate_ptr_array(fullexp, sizeof(expression), 5);
    allocate_array(stack, 1, 16);
    while (*c) {
        if (isalnum(*c) || *c == '.') {
            if (isdigit(*c) || *c == '.')
                buffer[endb++] = *c;
            else
                insert(fullexp, *c, 0, 0);
        } else if (*c == '+' || *c == '-' || *c == '*' || *c == '/' || *c == '^') {
            if (endb){
                buffer[endb]='\0';
                insert(fullexp, 0, atof(buffer), 1);
                endb=0;
            }
            if (stack.nmemb > 0)
                while (op[HASH(*c)] <= op[HASH(*(char*)AT_ARRAY(stack, stack.nmemb-1))]) {
                    stack.nmemb--;
                    insert(fullexp, *(char*)LAST_SPACE_ARRAY(stack), 0, 0);
                    if (stack.nmemb == 0)
                        break;
                }
            *(char*)LAST_SPACE_ARRAY(stack) = *c;
            stack.nmemb++;
            expand_array(stack,1,5);
        } else if (*c == '(') {
            *(char*)LAST_SPACE_ARRAY(stack) = *c;
            stack.nmemb++;
            expand_array(stack,1,5);
        } else if (*c == ')') {
            if (endb){
                buffer[endb]='\0';
                insert(fullexp, 0, atof(buffer), 1);
                endb=0;
            }
            while (*(char*)AT_ARRAY(stack, stack.nmemb-1) != '(') {
                stack.nmemb--;
                insert(fullexp, *(char*)LAST_SPACE_ARRAY(stack), 0, 0);
            }
            stack.nmemb--;
        }
        c++;
    }
    if (endb) {
        buffer[endb]='\0';
        insert(fullexp, 0, atof(buffer), 1);
        endb=0;
    }
    while (stack.nmemb) {
        stack.nmemb--;
        insert(fullexp, *(char*)LAST_SPACE_ARRAY(stack), 0, 0);
    }
    free(stack.base);
}

static dynamic_array stack = { 0 };

double evaluate(fullexp, x)
dynamic_array *fullexp;
double x;
{
    expression *next;
    double *op1, *op2, res;
    int i;
    
    if (!stack.capacity)
        allocate_array(stack, sizeof(double), 10);
    stack.nmemb=0;
    for (i=0; i < fullexp->nmemb; i++) {
        next = AT_PTR_ARRAY(fullexp, i);
        if (next->utype) {
            *(double*)LAST_SPACE_ARRAY(stack) = next->symb.fval;
            stack.nmemb++;
            expand_array(stack,1,5);
        } else {
            if (next->symb.opval == 'x') {
                *(double*)LAST_SPACE_ARRAY(stack) = x;
                stack.nmemb++;
                expand_array(stack,1,5);
            } else {
                op1 = AT_ARRAY(stack, stack.nmemb-2);
                op2 = AT_ARRAY(stack, stack.nmemb-1);
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
        }
    }
    res = *(double*)stack.base;
    return res;
}

void show_expr(fullexp)
dynamic_array *fullexp;
{
    expression *next;
    int i;

    for (i=0; i < fullexp->nmemb; i++) {
        next = AT_PTR_ARRAY(fullexp, i);
        if (next->utype)
            printf("|%lf|", next->symb.fval);
        else
            putchar(next->symb.opval);
    }
    putchar('\n');
}
