/*
    contingency_table - v4.2.0
    Prints a contingency table.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v4.2.0  Delimiter argument
        v4.1.1  Doubly precision in printf
        v4.1.0  Printing precision argument
        v4.0.0  Concatenates the readed files
        v3.0.0  Other format for printing
        v2.0.0  Translate into english
        v1.0.3  strsave() declaration
        v1.0.2  Changes in avltree_create
        v1.0.1  Changes in ARRAYTYPED_APPEND_TO_IDX_PTR
        v1.0.0  First version

    contingency_table is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "external/avltree/avltree.c"
#include "external/arrays/array_typed.c"

#include "getword.c"

#define TRUE 1
#define FALSE 0
#define LEFT 0
#define RIGHT 1
#define ROW_L (15+1)
#define FORMAT "%-15.15s"
#define SEPARATOR '|'

typedef struct {
    char *word;
    int count;
} frequencie;

typedef struct {
    frequencie *freqp;
    int count;
} attr;

compar_attr(x, y)
void *x, *y;
{
    return strcmp(((attr*)x)->freqp->word, ((attr*)y)->freqp->word);
}

ARRAYTYPED_GENERATE(attr, 2, 0)
ARRAYTYPED_GENERATE_ORDERED_INSERT(attr, 0, compar_attr)

typedef arraytyped_array_attr v_attr;

typedef struct {
    frequencie fword;
    v_attr *vattr;
} section;

typedef union { 
    section sword;
    frequencie fword;
} unode;

typedef unode *unode_tuple[2];

ARRAYTYPED_GENERATE(unode_tuple, 2, 0)

typedef arraytyped_array_unode_tuple array_relations_unode;

compar_f(x, y)
void *x, *y;
{
    return strcmp(((unode *)x)->fword.word, ((unode *)y)->fword.word);
}

compar_s(x, y)
void *x, *y;
{
    return strcmp(((unode *)x)->sword.fword.word,
                  ((unode *)y)->sword.fword.word);
}

int arg_columns = FALSE;
int arg_precision = 1;
char arg_delimiter = ';';

/* tf is a histogram of values and ts is a histogram of keys */
avltree_tree tf, ts;

main(argc, argv)
char *argv[];
{
    void read_attr(), make_table(), intersect(), free_intersect();
    FILE *fp;
    array_relations_unode U;

    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"column", no_argument, NULL, 'c'},
        {"delimiter", required_argument, NULL, 'd'},
        {"precision", required_argument, NULL, 'p'},
        { 0 },
    };
    int opt;

    for (; (opt = getopt_long(argc, argv, "cp:", long_opts, NULL)) != -1;)
        switch (opt) {
        case '?':
            fputs("Try 'contingency_table --help' for more information.\n",
                  stderr);
            return 2;
        case 'c':
            arg_columns = TRUE; 
            break;
        case 'd':
            arg_delimiter = *optarg;
            break;
        case 'p':
            arg_precision = atoi(optarg);
            break;
        case 'h':
            fputs("Usage: contingency_table [OPTION] [FILE]...\n"
                  "Prints a contingency table.\n\n"
                  "With no FILE read standard input.\n\n"
                  "  -c, --column            a column is a tuple\n"
                  "  -d, --delimiter=DELIM   use DELIM instead of semicolon for line delimiter, it\n"
                  "                          can't be a space character\n"
                  "  -p, --precision=NUM     printing precision of floating-point numbers, default\n"
                  "                          is 1\n",
                  stdout);
            return 0;
        }

    arraytyped_allocate(unode_tuple, U, 1);
    U.nmemb = 0;
    avltree_create(tf, 1, compar_f, NULL, NULL);
    avltree_create(ts, 1, compar_s, NULL, NULL);

    if (optind == argc)
        read_attr(stdin, &U);
    else
        while (optind < argc)
            if ((fp = fopen(argv[optind++], "r"))) {
                read_attr(fp, &U);
                fclose(fp);
            } else {
                fprintf(stderr, "contingency_table: file not found: %s\n", argv[optind-1]);
                return EXIT_FAILURE;
            }

    intersect(&ts, &U);
    make_table(stdout, &ts, &tf);

    /* releases memory */
    free_intersect(ts.root);
    avltree_destroy(tf);
    avltree_destroy(ts);
    return 0;
}

int first;

void read_attr(fp, arr)
FILE *fp;
array_relations_unode *arr;
{
    extern char *strsave();
    void insert_tuple(), make_table(), insert(), intersect();
    char s[MAXLEN];
    int type, side = LEFT;
    avltree_node *ret;
    unode key, *new;
    
    first = FALSE;
    while ((type = getword(s, MAXLEN, fp)) != EOF)
        if (type == LETTER) {
            if (side) {
                key.fword.word = s;
                if ((ret = avltree_find_node(tf, &key)))
                    ((frequencie*)ret->key)->count++;
                else {
                    new = malloc(sizeof(unode));
                    new->fword.word = strsave(s);
                    new->fword.count = 1;
                    ret = avltree_insert_key(tf, new);
                }
            } else {
                key.sword.fword.word = s;
                if ((ret = avltree_find_node(ts, &key))) {
                    ((section*)ret->key)->fword.count++;
                } else {
                    new = malloc(sizeof(unode));
                    new->sword.fword.word = strsave(s);
                    new->sword.fword.count = 1;
                    new->sword.vattr = NULL;
                    ret = avltree_insert_key(ts, new);
                }
            }
            insert_tuple(ret, arr, side);
            if (arg_columns == FALSE)
                side = !side;
        } else if (type == arg_delimiter && arg_columns)
            side = RIGHT;
}

void insert_tuple(x, arr, side)
avltree_node *x;
array_relations_unode *arr;
{
    if (side == RIGHT && arg_columns && first == FALSE) {
        arr->nmemb = 0;
        first = TRUE;
    }
    arraytyped_expand_unode_tuple(arr);
    (*ARRAYTYPED_LAST_SPACE_PTR(arr))[side] = x->key;
    if ((arg_columns==FALSE && side==RIGHT) || arg_columns)
        arr->nmemb++;
}

/* Creates a histogram with all values that some key have and stores in vattr
 * of that key. */
void intersect(t, arr)
avltree_tree *t;
array_relations_unode *arr;
{
    unode_tuple *ptr;
    attr item;
    v_attr **vec;
    size_t idx;

    for (ptr=arr->base; ptr < arr->base + arr->nmemb; ptr++) {
        vec = &((*ptr)[LEFT])->sword.vattr;
        if (!*vec) {
            *vec = malloc(sizeof(v_attr));
            arraytyped_allocate_ptr(attr, (*vec),  2);
        }
        item.freqp = &((*ptr)[RIGHT])->fword;
        item.count = 1;
        if (!arraytyped_insert_aorder_attr(*vec, &item, &idx))
            ARRAYTYPED_AT_PTR((*vec), idx)->count++;
    }
    free(arr->base);
}

void free_intersect(r)
avltree_node *r;
{
    if (!r) return;
    free_intersect(r->child[0]);
    free_intersect(r->child[1]);
    free(((unode*)(r->key))->sword.vattr);
}

ARRAYTYPED_GENERATE(int, 2, 0)

typedef char *str;

ARRAYTYPED_GENERATE(str, 2, 0)

void make_table(fp, ts, tf)
FILE *fp;
avltree_tree *ts, *tf;
{
    void theader(), trow();
    arraytyped_array_int row, total;
    arraytyped_array_str header;
    char cell[ROW_L];
    int i, n = 0;
    
    arraytyped_allocate(int, row, 2);
    arraytyped_allocate(int, total, 2);
    arraytyped_allocate(str, header, 2);

    fprintf(fp, FORMAT, "Rank"); putc(SEPARATOR, fp);
    theader(tf->root, &header, &total, &n);
    for (i=0; i<header.nmemb; i++) {
        fprintf(fp, FORMAT, header.base[i]); putc(SEPARATOR, fp);
    }
    fprintf(fp, "Total\n");
    trow(fp, ts->root, &row, &header, cell);
    fprintf(fp, FORMAT, "Total"); putc(SEPARATOR, fp);
    for (i=0; i<total.nmemb; i++) {
        sprintf(cell, "%d (%.*lf%%)", total.base[i], arg_precision,
                ((double)total.base[i]*100)/n);
        fprintf(fp, FORMAT, cell); putc(SEPARATOR, fp);
    }
    fprintf(fp, "%d (%.*lf%%)\n", n, arg_precision, 100.);

    free(row.base);
    free(total.base);
    free(header.base);
}

/* acc gets the total of tuples, header gets all types of values sorted and
 * total gets the histogram of values in an array. */
void theader(r, header, total, acc)
avltree_node *r;
arraytyped_array_str *header;
arraytyped_array_int *total;
int *acc;
{
    int count;

    if (r) {
        theader(r->child[0], header, total, acc);
        arraytyped_append_to_end_ptr(str, header, &((unode*)r->key)->fword.word);
        count = ((unode*)r->key)->fword.count;
        *acc += count;
        arraytyped_append_to_end_ptr(int, total, &count);
        theader(r->child[1], header, total, acc);
    }
}

/* Prints the histogram of each key with their values and the fractions. */
void trow(fp, r, e, h, ce)
FILE *fp;
avltree_node *r;
arraytyped_array_int *e;
arraytyped_array_str *h;
char *ce;
{
    int i, j, n;

    if (r) {
        trow(fp, r->child[0], e, h, ce);
        fprintf(fp, FORMAT, ((unode*)r->key)->sword.fword.word);
        for (i=j=e->nmemb=n=0; j < h->nmemb; j++) {
            /* If the values are different, the key doesn't have any tuple with
             * that value, so gets 0. If the vattr ended, then all other values
             * get 0. */
            if (i == ((unode*)r->key)->sword.vattr->nmemb ||
                strcmp(((unode*)r->key)->sword.vattr->base[i].freqp->word, h->base[j])) {
                e->base[e->nmemb++] = 0;
                arraytyped_expand_int(e);
                continue;
            }
            e->base[e->nmemb] = ((unode*)r->key)->sword.vattr->base[i++].count;
            /* n gets the total of tuples with that key. */
            n += e->base[e->nmemb++];
            arraytyped_expand_int(e);
        }
        putc(SEPARATOR, fp);
        for (i=0; i < e->nmemb; i++) {
            sprintf(ce, "%d (%.*lf%%)", e->base[i], arg_precision,
                    ((double)e->base[i]*100)/n);
            fprintf(fp, FORMAT, ce); putc(SEPARATOR, fp);
        }
        fprintf(fp, "%d (%.*lf%%)\n", n, arg_precision, 100.);
        trow(fp, r->child[1], e, h, ce);
    }
}
