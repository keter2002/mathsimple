/*
    contingency_table - v1.0.3
    Contingency table implementation in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.0.3  strsave() declaration.
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

#include "external/avltree/avltree.c"
#include "external/arrays/array_typed.c"

#include "getword.c"

#define TRUE 1
#define FALSE 0
#define LEFT 0
#define RIGHT 1
#define DELIMITER ';'

int columns = FALSE;

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

/*polir o funcionamento e os nomes da trow*/
/*fazer o resumo do funcionamento de cada funcao*/

main(argc, argv)
char *argv[];
{
    void read_attr();
    FILE *fp;
    array_relations_unode U;
    
    arraytyped_allocate(unode_tuple, U, 1);
    if (argc >= 2)
        if (argv[1][0] == '-') {
            argc--;
            if ((*++argv)[1] == 'c')
                columns = TRUE;
            else
                fprintf(stderr, "contingency_table: invalid argument (%c)\n", argv[1][1]);
        }
    if (argc <= 1)
        read_attr(stdin, &U);
    else
        while (--argc > 0) {
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "contingency_table: file not found (%s)\n", *argv);
                return 1;
            }
            read_attr(fp, &U);
            fclose(fp);
        }
}

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

int first;

void read_attr(fp, arr)
FILE *fp;
array_relations_unode *arr;
{
    extern char *strsave();
    void insert_tuple(), make_table(), insert(), intersect();
    char s[MAXLEN];
    int type, side = LEFT;
    avltree_tree tf, ts;
    avltree_node *ret;
    unode key, *new;
    
    avltree_create(tf, 1, compar_f, NULL, NULL);
    avltree_create(ts, 1, compar_s, NULL, NULL);
    arr->nmemb = 0;
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
            if (columns == FALSE)
                side = !side;
        } else if (type == DELIMITER && columns)
            side = RIGHT;
    intersect(&ts, arr);
    make_table(&ts, &tf);
}

void insert_tuple(x, arr, side)
avltree_node *x;
array_relations_unode *arr;
{
    if (side == RIGHT && columns && first == FALSE) {
        arr->nmemb = 0;
        first = TRUE;
    }
    arraytyped_expand_unode_tuple(arr);
    (*ARRAYTYPED_LAST_SPACE_PTR(arr))[side] = x->key;
    if ((columns==FALSE && side==RIGHT) || columns)
        arr->nmemb++;
}

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

ARRAYTYPED_GENERATE(int, 2, 0)

typedef char *str;

ARRAYTYPED_GENERATE(str, 2, 0)

#define ROW_L 15
#define FORMAT "|%-15.15s|"

void make_table(ts, tf)
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

    printf(FORMAT, "Categoria");
    theader(tf->root, &header, &total, &n);
    for (i=0; i<header.nmemb; i++)
        printf(FORMAT, header.base[i]);
    printf(FORMAT, "Total");
    putchar('\n');
    trow(ts->root, &row, &header, cell);
    printf(FORMAT, "Total");
    for (i=0; i<total.nmemb; i++) {
        sprintf(cell, "%d (%.1f%%)", total.base[i], ((float)total.base[i]*100)/n);
        printf(FORMAT, cell);
    }
    sprintf(cell, "%d (100,0%%)", n);
    printf(FORMAT, cell);
    putchar('\n');

    free(row.base);
    free(total.base);
    free(header.base);
}

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

void trow(r, e, h, ce)
avltree_node *r;
arraytyped_array_int *e;
arraytyped_array_str *h;
char *ce;
{
    int i, j, n;

    if (r) {
        trow(r->child[0], e, h, ce);
        printf(FORMAT, ((unode*)r->key)->sword.fword.word);
        for (i=j=e->nmemb=n=0; j < h->nmemb; j++) {
            if (strcmp(((unode*)r->key)->sword.vattr->base[i].freqp->word, h->base[j])) {
                e->base[e->nmemb++] = 0;
                arraytyped_expand_int(e);
                continue;
            }
            e->base[e->nmemb] = ((unode*)r->key)->sword.vattr->base[i++].count;
            n += e->base[e->nmemb++];
            arraytyped_expand_int(e);
        }
        for (i=0; i < e->nmemb; i++) {
            sprintf(ce, "%d (%.1f%%)", e->base[i], ((float)e->base[i]*100)/n);
            printf(FORMAT, ce);
        }
        sprintf(ce, "%d (100.0%%)", n);
        printf(FORMAT, ce);
        putchar('\n');
        trow(r->child[1], e, h, ce);
    }
}
