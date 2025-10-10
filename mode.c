/*
    mode - v3.0.0
    Prints central tendency measures.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
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
#include <getopt.h>
#include <math.h>

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

int print_values = 0;

int main(argc, argv)
int argc;
char *argv[];
{
    minmaxh_heap_frequencie *construct_heap();
    void estatistics(), dsc_print();
    FILE *fp;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"values", no_argument, NULL, 'v'},
        { 0 },
    };
    int opt;
    
    switch ((opt = getopt_long(argc, argv, "v", long_opts, NULL))) {
    case 'v':
        print_values = TRUE;
        argc--;
        argv++;
        break;
    case '?':
        fputs("Try 'mode --help' for more information.\n", stderr);
        return 2;
    case 'h':
        fputs("Usage: mode [OPTION] [FILE]...\n"
              "Prints central tendency measures.\n\n"
              "With no FILE read standard input.\n\n"
              "  -v, --values    prints all values readed\n",
              stderr);
        return 0;
    }

    arraytyped_allocate(double, v, 1);
    avltree_create(tree, 1, mathfn_compar_double, NULL, NULL);
    v.nmemb = 0;

    if (argc <= 1)
        read_nums(stdin);
    else
        while (--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "mode: file not found: %s\n", *argv);
                return EXIT_FAILURE;
            } else {
                read_nums(fp);
                fclose(fp);
            }

    arraytyped_qsort(double, v, mathfn_compar_double);
    estatistics(stdout, v.base, v.nmemb);

    dsc_print(stdout, construct_heap(&tree));
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

void estatistics(stream, v, size)
FILE *stream;
double *v;
int size;
{
    double avg = 0.0, dev = 0.0, q1, q3;
    extern double mathfn_roundd();
    double *aux;
    
    if (print_values) fprintf(stream, "Values: ");
    for (aux=v; aux < v+size; aux++) {
        if (print_values)
            fprintf(stream, "%.2lf%c", *aux, (aux==v+size-1)? '\n' : ' ');
        avg += *aux;
    }
    fprintf(stream, "Mean: %.2lf\n", avg=mathfn_roundd(avg/=size));
    for (aux=v; aux < v+size; aux++)
        dev += mathfn_roundd(mathfn_powd(*aux-avg, 2));
    fprintf(stream, "SD: %.2lf\n", dev=mathfn_roundd(sqrt(mathfn_roundd(dev/(size-1)))));
    fprintf(stream, "Range: %.2lf\n", mathfn_roundd(v[size-1]-v[0]));
    fprintf(stream, "Variance: %.2lf\n", mathfn_roundd(mathfn_powd(dev, 2)));
    fprintf(stream, "CV: %.2lf%%\n", mathfn_roundd(dev*100/avg));
    fprintf(stream, "Median: %.2lf\n", (size%2==0)? mathfn_roundd((v[((size-1)>>1)+1]+v[(size-1)>>1])/2) : v[size>>1]);
    fprintf(stream, "Q1: %.2lf\n", q1 = size%4==0? mathfn_roundd((v[((size-1)>>2)+1]+v[(size-1)>>2])/2) : v[(int)ceil((size-1)>>2)]);
    fprintf(stream, "Q3: %.2lf\n", q3 = size*3%4==0? mathfn_roundd((v[(size*3>>2)-1]+v[(size*3>>2)])/2) : v[(int)ceil(size*3>>2)]);
    fprintf(stream, "IQR: %.2lf\n", q3-q1);
}

void dsc_print(stream, h)
FILE *stream;
minmaxh_heap_frequencie *h;
{
    frequencie p;
    double last;
    int amodal = TRUE;

    minmaxh_remove_max_frequencie(h, &p);
    /*minmaxh_remove_min_frequencie(h, &p);*/
    while (p.count > 1) {
        amodal = FALSE; 
        fprintf(stream, "%4d %lf\n", p.count, p.value);
        last = p.count;
        minmaxh_remove_max_frequencie(h, &p);
        /*minmaxh_remove_min_frequencie(h, &p);*/
        if (last != p.count)
            break;
    }
    if (amodal)
        fputs("Amodal\n", stream);
    free(h->base);
    free(h);
}
