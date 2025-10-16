/*
    mode - v3.1.0
    Prints central tendency measures.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v4.0.0  verbose, not values argument
        v3.1.1  help in stdout
        v3.1.0  Printing precision argument
        v3.0.0  Concatenates all input files readed and flag for printing
                values
        v2.0.0  Translate into english
        v1.0.5  Remove torfnum.h
        v1.0.4  atof() replaces torfnum_atof()
        v1.0.3  torfnum_atof macro
        v1.0.2  Changes in avltree_create
        v1.0.1  Replace ARRAYTYPED_APPEND_TO_IDX with arraytyped_append_to_end
                and arraytyped_quick_sort_double with arraytyped_qsort
        v1.0.0  First version

    mode is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <getopt.h>

#include "external/avltree/avltree.c"
#include "external/heaps/minmax_heap_typed.c"
#include "external/arrays/array_typed.c"

#include "mathfn.h"
#include "getop.c"

#define TRUE 1
#define FALSE 0
#define STEP 2

ARRAYTYPED_GENERATE(double, 2, 0)

arraytyped_array_double v;
avltree_tree tree;

typedef struct {
    double value;
    int count;
} frequencie;

compar_frequencie(x, y)
void *x, *y;
{
    return ((frequencie*)x)->count - ((frequencie*)y)->count;
}

MINMAXH_GENERATE(frequencie, compar_frequencie)

void read_nums(fp)
FILE *fp;
{
    char s[MAXOP];
    int type, *count;
    double *x;
    avltree_node *found;

    while ((type = getop(s, MAXOP, fp)) != EOF)
        if (type == NUMBER) {
            x = malloc(sizeof(double));
            *x = atof(s);
            if ((found = avltree_find_node(tree, x)))
                *(int*)(found->value) += 1;
            else {
                count = malloc(sizeof(int));
                *count = 1;
                avltree_insert(&tree, x, count);
            }
            arraytyped_append_to_end(double, v, x);
        }
}

int arg_verbose = 0;
int arg_precision = 2;

int main(argc, argv)
int argc;
char *argv[];
{
    minmaxh_heap_frequencie *construct_heap();
    void statistics(), dsc_print();
    FILE *fp;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"verbose", no_argument, NULL, 'v'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    int opt;

    for (; (opt = getopt_long(argc, argv, "p:v", long_opts, NULL)) != -1;)
        switch (opt) {
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case 'v':
            arg_verbose = TRUE;
            break;
        case '?':
            fputs("Try 'mode --help' for more information.\n", stderr);
            return 2;
        case 'h':
            fputs("Usage: mode [OPTION]... [FILE]...\n"
                  "Prints central tendency measures.\n\n"
                  "With no FILE read standard input.\n\n"
                  "  -v, --verbose      prints all values readed\n"
                  "  -p, --precision    printing precision of floating-point numbers, default is 2\n",
                  stdout);
            return 0;
        }

    arraytyped_allocate(double, v, 1);
    avltree_create(tree, 1, mathfn_compar_double, NULL, NULL);
    v.nmemb = 0;

    if (optind == argc)
        read_nums(stdin);
    else
        while (optind < argc)
            if ((fp = fopen(argv[optind++], "r"))) {
                read_nums(fp);
                fclose(fp);
            } else {
                fprintf(stderr, "mode: file not found: %s\n", argv[optind-1]);
                return EXIT_FAILURE;
            }

    arraytyped_qsort(double, v, mathfn_compar_double);
    statistics(stdout, v.base, v.nmemb);
    dsc_print(stdout, construct_heap(&tree));

    free(v.base);
    avltree_destroy(tree);
    return 0;
}

struct qnode {
    avltree_node *n;
    struct qnode *next;
};

struct queue {
    struct qnode *first, *last;
    int qtt;
};

static void push(q, p)
struct queue *q;
avltree_node *p;
{
    struct qnode *new = malloc(sizeof(struct qnode));
        
    new->n = p;
    new->next = NULL;
    if (q->qtt == 0)
        q->last = q->first = new;
    else {
        q->last->next = new;
        q->last = new;
    }
    q->qtt++;
}


static struct qnode *pop(q)
struct queue *q;
{
    struct qnode *p = q->first;
        
    q->qtt--;
    q->first = q->first->next;
    return (p);
}

minmaxh_heap_frequencie *construct_heap(p)
avltree_tree *p;
{
    struct qnode *qn;
    struct queue q;
    minmaxh_heap_frequencie *h;
    frequencie newf;
    
    h = malloc(sizeof(minmaxh_heap_frequencie));
    minmaxh_allocate_ptr(frequencie, h, p->nmemb);
    q.qtt = 0;
    if (!p->root)
        return h;
    push(&q, p->root);
    while (q.qtt) {
        qn = pop(&q);
        if (qn->n->child[0])
            push(&q, qn->n->child[0]);
        if (qn->n->child[1])
            push(&q, qn->n->child[1]);
        newf.value = *(double*)(qn->n->key);
        newf.count = *(int*)(qn->n->value);
        minmaxh_insert_frequencie(h, &newf);
        free(qn);
    }
    return h;
}

void statistics(stream, v, nmemb)
FILE *stream;
double *v;
{
    double avg = 0.0, dev = 0.0, q1, q3;
    extern double mathfn_roundd();
    double *aux;
    
    if (arg_verbose)
        fprintf(stream, "Values:%c", nmemb? ' ' : '\n');
    for (aux=v; aux < v+nmemb; aux++) {
        if (arg_verbose)
            fprintf(stream, "%.*lf%c", arg_precision, *aux, (aux==v+nmemb-1)?
                    '\n' : ' ');
        avg += *aux;
    }
    fprintf(stream, "Mean: %.*lf\n", arg_precision, nmemb?
            avg=mathfn_roundd(avg/=nmemb) : 0.);
    for (aux=v; aux < v+nmemb; aux++)
        dev += mathfn_roundd(mathfn_powd(*aux-avg, 2));
    fprintf(stream, "SD: %.*lf\n", arg_precision,
            dev=mathfn_roundd(sqrt(mathfn_roundd(dev/(nmemb-1)))));
    fprintf(stream, "Range: %.*lf\n", arg_precision,
            mathfn_roundd(v[nmemb-1]-v[0]));
    fprintf(stream, "Variance: %.*lf\n", arg_precision,
            mathfn_roundd(mathfn_powd(dev, 2)));
    fprintf(stream, "CV: %.*lf%%\n", arg_precision, nmemb?
            mathfn_roundd(dev*100/avg) : 0.);
    fprintf(stream, "Median: %.*lf\n", arg_precision, (nmemb%2==0)?
            mathfn_roundd((v[((nmemb-1)>>1)+1]+v[(nmemb-1)>>1])/2) : v[nmemb>>1]);
    fprintf(stream, "Q1: %.*lf\n", arg_precision, q1 = nmemb%4==0?
            mathfn_roundd((v[((nmemb-1)>>2)+1]+v[(nmemb-1)>>2])/2) :
            v[(int)ceil((nmemb-1)>>2)]);
    fprintf(stream, "Q3: %.*lf\n", arg_precision, q3 = nmemb*3%4==0?
            mathfn_roundd((v[(nmemb*3>>2)-1]+v[(nmemb*3>>2)])/2) :
            v[(int)ceil(nmemb*3>>2)]);
    fprintf(stream, "IQR: %.*lf\n", arg_precision, q3-q1);
}

void dsc_print(stream, h)
FILE *stream;
minmaxh_heap_frequencie *h;
{
    frequencie p;
    double last;
    int amodal = TRUE;
    
    if (h->nmemb) {
        minmaxh_remove_max_frequencie(h, &p);
        /*minmaxh_remove_min_frequencie(h, &p);*/
        while (p.count > 1) {
            amodal = FALSE; 
            fprintf(stream, "%4d %.*lf\n", p.count, arg_precision, p.value);
            last = p.count;
            minmaxh_remove_max_frequencie(h, &p);
            /*minmaxh_remove_min_frequencie(h, &p);*/
            if (last != p.count)
                break;
        }
    }
    if (amodal) fputs("Amodal\n", stream);
    free(h->base);
    free(h);
}
