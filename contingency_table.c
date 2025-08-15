/*
    Contingency table implementation in C.
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
#include <stdlib.h>
#include "getword.c"
#include "external/avltree/avltree.c"
#include "external/arrays/array_typed.c"

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

GENERATE_ARRAYTYPED(attr, 2, 0)
GENERATE_ORDERED_INSERT_ARRAYTYPED(attr, 0, compar_attr)

typedef array_arraytyped_attr v_attr;

typedef struct {
    frequencie fword;
    v_attr *vattr;
} section;

typedef union { 
    section sword;
    frequencie fword;
} unode;

typedef unode *unode_tuple[2];

GENERATE_ARRAYTYPED(unode_tuple, 2, 0)

typedef array_arraytyped_unode_tuple array_relations_unode;

/*polir o funcionamento e os nomes da trow*/
/*fazer o resumo do funcionamento de cada funcao*/

main(argc, argv)
char *argv[];
{
    void read_attr();
    FILE *fp;
    array_relations_unode U;
    
    allocate_arraytyped(unode_tuple, U, 1);
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
    void insert_tuple(), make_table(), insert(), intersect();
    char s[MAXLEN];
    int type, side = LEFT;
    avl_tree tf, ts;
    avl_node *ret;
    unode key, *new;
    
    create_avltree(tf, 1, compar_f, NULL);
    create_avltree(ts, 1, compar_s, NULL);
    arr->nmemb = 0;
    first = FALSE;
    while ((type = getword(s, MAXLEN, fp)) != EOF)
        if (type == LETTER) {
            if (side) {
                key.fword.word = s;
                if ((ret = find_node_avltree(tf, &key)))
                    ((frequencie*)ret->key)->count++;
                else {
                    new = malloc(sizeof(unode));
                    new->fword.word = strsave(s);
                    new->fword.count = 1;
                    ret = insert_key_avltree(tf, new);
                }
            } else {
                key.sword.fword.word = s;
                if ((ret = find_node_avltree(ts, &key))) {
                    ((section*)ret->key)->fword.count++;
                } else {
                    new = malloc(sizeof(unode));
                    new->sword.fword.word = strsave(s);
                    new->sword.fword.count = 1;
                    new->sword.vattr = NULL;
                    ret = insert_key_avltree(ts, new);
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
avl_node *x;
array_relations_unode *arr;
{
    if (side == RIGHT && columns && first == FALSE) {
        arr->nmemb = 0;
        first = TRUE;
    }
    expand_arraytyped_unode_tuple(arr);
    (*LAST_SPACE_PTR_ARRAYTYPED(arr))[side] = x->key;
    if ((columns==FALSE && side==RIGHT) || columns)
        arr->nmemb++;
}

void intersect(t, arr)
avl_tree *t;
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
            allocate_ptr_arraytyped(attr, (*vec),  2);
        }
        item.freqp = &((*ptr)[RIGHT])->fword;
        item.count = 1;
        if (!insert_aorder_arraytyped_attr(*vec, &item, &idx))
            AT_PTR_ARRAYTYPED((*vec), idx)->count++;
    }
    free(arr->base);
}

GENERATE_ARRAYTYPED(int, 2, 0)

typedef char *str;

GENERATE_ARRAYTYPED(str, 2, 0)

#define ROW_L 15
#define FORMAT "|%-15.15s|"

void make_table(ts, tf)
avl_tree *ts, *tf;
{
    void theader(), trow();
    array_arraytyped_int row, total;
    array_arraytyped_str header;
    char cell[ROW_L];
    int i, n = 0;
    
    allocate_arraytyped(int, row, 2);
    allocate_arraytyped(int, total, 2);
    allocate_arraytyped(str, header, 2);

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
avl_node *r;
array_arraytyped_str *header;
array_arraytyped_int *total;
int *acc;
{
    int count;

    if (r) {
        theader(r->child[0], header, total, acc);
        APPEND_TO_IDX_PTR_ARRAYTYPED(str, header, header->nmemb-1, &((unode*)r->key)->fword.word);
        count = ((unode*)r->key)->fword.count;
        *acc += count;
        APPEND_TO_IDX_PTR_ARRAYTYPED(int, total, total->nmemb-1, &count);
        theader(r->child[1], header, total, acc);
    }
}

void trow(r, e, h, ce)
avl_node *r;
array_arraytyped_int *e;
array_arraytyped_str *h;
char *ce;
{
    int i, j, n;

    if (r) {
        trow(r->child[0], e, h, ce);
        printf(FORMAT, ((unode*)r->key)->sword.fword.word);
        for (i=j=e->nmemb=n=0; j < h->nmemb; j++) {
            if (strcmp(((unode*)r->key)->sword.vattr->base[i].freqp->word, h->base[j])) {
                e->base[e->nmemb++] = 0;
                expand_arraytyped_int(e);
                continue;
            }
            e->base[e->nmemb] = ((unode*)r->key)->sword.vattr->base[i++].count;
            n += e->base[e->nmemb++];
            expand_arraytyped_int(e);
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
