/*
    avltree.c - v2.0.0
    AVL tree implementation in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.0.0  avl_diff(), avl_copy_keys(), flags parameter in avl_remove,
                stream pointer parameter and separator string in printing
                routines.
        v1.1.0  avl_empty()
        v1.0.0  First version

    avltree.c is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "avltree.h"


void avl_diff(td, ts, r, flags)
avltree_tree *td, *ts;
avltree_node *r;
unsigned char flags;
{
    if (!r)
        return;
    avl_diff(td, ts, r->child[0], flags);
    avl_diff(td, ts, r->child[1], flags);
    avltree_remove_node_ptr(td, r->key, flags);
}

void avl_empty(t, r)
avltree_tree *t;
avltree_node *r;
{
    if (!r)
        return;
    avl_empty(t, r->child[0]);
    avl_empty(t, r->child[1]);
    free(r);
}

void avl_copy_keys(td, ts, r)
avltree_tree *td, *ts;
avltree_node *r;
{
    if (!r)
        return;
    avl_copy_keys(td, ts, r->child[0]);
    avl_copy_keys(td, ts, r->child[1]);
    avltree_insert(td, r->key, NULL);
}

void avl_destroy(t, r)
avltree_tree *t;
avltree_node *r;
{
    if (!r)
        return;
    avl_destroy(t, r->child[0]);
    avl_destroy(t, r->child[1]);
    free(r->key);
    if (r->has_value) free(r->value);
    free(r);
}

avltree_node *avl_find_max(t, r)
avltree_tree *t;
avltree_node *r;
{
    if (!r) return NULL;
    if (r->child[1])
        return avl_find_max(t, r->child[1]);
    return r;
}

avltree_node *avl_find_min(t, r)
avltree_tree *t;
avltree_node *r;
{
    if (!r) return NULL;
    if (r->child[0])
        return avl_find_min(t, r->child[0]);
    return r;
}

static avltree_node *find_leaf_avl(t, r, key)
avltree_tree *t;
avltree_node *r;
void *key;
{
    int gt;

    if (!r) return NULL;
    gt = t->compar(key, r->key) > 0;
    if (!r->child[gt])
        return r;
    return find_leaf_avl(t, r->child[gt], key);
}

avltree_node *avl_find_node(t, r, key, parent)
avltree_tree *t;
avltree_node *r, **parent;
void *key;
{
    int cmp;
    
    if (!r) return NULL;
    if (!(cmp = t->compar(key, r->key)))
        return r;
    if (parent)
        *parent = r;
    return avl_find_node(t, r->child[cmp > 0], key, parent);
}

/* Least mirroed alg. */
static void retrace(t, x, z, inc, removed)
avltree_tree *t;
avltree_node *x, *z;
unsigned char removed;
{
    int orig_bf, bf_z0, bf_y0;
    unsigned char side_x;
    avltree_node *y;

    if (!x)
        return;

    orig_bf = x->bf;
    x->bf += inc;
    
    /* Apply rotations. */
    if (x->parent)
        side_x = x->parent->child[1] == x;
    /* R */
    if (x->bf/2 > 0) {
        bf_z0 = z->bf;
        /* R */
        if (bf_z0 >= 0) {
            if (x->parent)
                x->parent->child[side_x] = z;
            else
                t->root = z;
            z->parent = x->parent;
            x->child[1] = z->child[0];
            if (z->child[0])
                z->child[0]->parent = x;
            z->child[0] = x;
            x->parent = z;
            
            /* Update BFs. */
            x->bf = -bf_z0 + 1;
            z->bf = bf_z0 - 1;
            /* New x: */
            x = z;
        /* L */
        } else {
            y = z->child[0];
            if (x->parent)
                x->parent->child[side_x] = y;
            else
                t->root = y;
            y->parent = x->parent;
            x->child[1] = y->child[0];
            if (y->child[0])
                y->child[0]->parent = x;
            y->child[0] = x;
            x->parent = y;
            z->child[0] = y->child[1];
            if (y->child[1])
                y->child[1]->parent = z;
            y->child[1] = z;
            z->parent = y;
            
            /* Update BFs. */
            bf_y0 = y->bf;
            if (bf_y0 < 0) {
                x->bf = 0;
                z->bf = 1;
            } else {
                x->bf = -bf_y0;    
                z->bf = 0;
            }
            y->bf = 0;
            /* New x: */
            x = y;
        }
    /* L */
    } else if (x->bf/2 < 0) {
        bf_z0 = z->bf;
        /* R */
        if (bf_z0 > 0) {
            y = z->child[1];
            if (x->parent)
                x->parent->child[side_x] = y;
            else
                t->root = y;
            y->parent = x->parent;
            x->child[0] = y->child[1];
            if (y->child[1])
                y->child[1]->parent = x;
            y->child[1] = x;
            x->parent = y;
            z->child[1] = y->child[0];
            if (y->child[0])
                y->child[0]->parent = z;
            y->child[0] = z;
            z->parent = y;
            
            /* Update BFs. */
            bf_y0 = y->bf;
            if (bf_y0 > 0) {
                x->bf = 0;
                z->bf = -1;
            } else {
                x->bf = -bf_y0;    
                z->bf = 0;
            }
            y->bf = 0;
            /* New x: */
            x = y;
        /* L */
        } else {
            if (x->parent)
                x->parent->child[side_x] = z;
            else
                t->root = z;
            z->parent = x->parent;
            x->child[0] = z->child[1];
            if (z->child[1])
                z->child[1]->parent = x;
            z->child[1] = x;
            x->parent = z;
            
            /* Update BFs. */
            x->bf = -bf_z0 - 1;
            z->bf = bf_z0 + 1;
            /* New x: */
            x = z;
        }
    }
    /* If it balanced, it means that the increment or decrement was canceled */
    /* out for the ancestors. */
    if (removed ^ (x->bf != 0)) {
        inc = abs(x->bf - orig_bf);
        retrace(t, x->parent, x->parent? x->parent->child[removed? x->parent->bf > 0: side_x] : x,
                removed ^ side_x? inc : -inc, removed);
    }
}

avltree_node *avltree_insert(t, key, value)
avltree_tree *t;
void *key, *value;
{
    avltree_node *node, *parent, *new;
    int gt;
    
    /* Commom insert in bst. **** */
    parent = NULL;
    if ((node = avl_find_node(t, t->root, key, &parent)) && t->inplace) {
        if (node->has_value) {
            node->has_value = 0;
            free(node->value);
        }
        if (value) {
            node->has_value = 1;
            node->value = value;
        }
        free(key);
        return node;
    } else
        parent = find_leaf_avl(t, node? node : t->root, key);
    if (parent) {
        gt = t->compar(key, parent->key) > 0;
        new = parent->child[gt] = malloc(sizeof(avltree_node));
    } else
        new = t->root = malloc(sizeof(avltree_node));
    new->parent = parent;
    new->key = key;
    if (value) {
        new->value = value;
        new->has_value = 1;
    } else
        new->has_value = 0;
    new->child[0] = new->child[1] = NULL;
    new->bf = 0;
    t->nmemb++;
    /* **** */
    if (parent)
        retrace(t, parent, new, gt * 2 - 1, 0);
    return new;
}

void avl_remove(t, z, flags)
avltree_tree *t;
avltree_node *z;
unsigned char flags;
{
    avltree_node *y, *q, *parenty;
    unsigned char side_z, two;
    int inc;

    assert(z);
    q = z->parent;
    two = 0;
    if (z->child[0] && z->child[1]) {
        y = avl_find_max(t, z->child[0]);
        parenty = y->parent;
        if (parenty != z) {
            parenty->child[1] = y->child[0];
            if (y->child[0])
                y->child[0]->parent = parenty;
            y->child[0] = z->child[0];
            z->child[0]->parent = y;
        }
        y->child[1] = z->child[1];
        z->child[1]->parent = y;
        two = 1;
        /* Update Y BF */
        y->bf = z->bf;
    } else
        y = z->child[0]? z->child[0] : z->child[1];
    if (q) {
        side_z = q->child[1] == z;
        q->child[side_z] = y;    
    } else
        t->root = y;
    if (y)
        y->parent = q;
    
    if (flags & AVLTREE_FREE_VALUE && z->has_value)
        free(z->value);
    if (flags & AVLTREE_FREE_KEY)
        free(z->key);
    free(z);
    t->nmemb--;

    /* Update BFs */
    if (y && two) {
        if (parenty == z) {
            parenty = y;
            inc = 1;
        } else
            inc = -1;
        retrace(t, parenty, parenty->child[parenty->bf > 0], inc, 1);
    } else if (q)
        retrace(t, q, q->child[!(q->bf || side_z) ||
                q->bf > 0],
                side_z? -1 : 1, 1);
}

avltree_remove(t, key, flags)
avltree_tree *t;
void *key;
unsigned char flags;
{
    avltree_node *z;
    
    if (!(z = avl_find_node(t, t->root, key, NULL)))
        return 1;
    avl_remove(t, z, flags);
    return 0;
}

/* Printing routines: */

void avl_infix(stream, t, r, last)
FILE *stream;
avltree_tree *t;
avltree_node *r, *last;
{
    if (!r)
        return;
    avl_infix(stream, t, r->child[0], last);
    if (t->stprint.print_fn)
        t->stprint.print_fn(stream, r->key, r->value);
    if (last != r)
        fprintf(stream, "%s", t->stprint.separator);
    avl_infix(stream, t, r->child[1], last);
}

void avl_dump_infix(stream, t, r, last)
FILE *stream;
avltree_tree *t;
avltree_node *r, *last;
{
    if (!r)
        return;
    avl_dump_infix(stream, t, r->child[0], last);
    fprintf(stream, "%p ", r);
    if (t->stprint.print_fn)
        t->stprint.print_fn(stream, r->key, r->value);
    fprintf(stream, " [%d] (%p, %p) ^%p", r->bf, r->child[0], r->child[1], r->parent);
    if (last != r)
        putc('\n', stream);
    avl_dump_infix(stream, t, r->child[1], last);
}

void avl_prefix(stream, t, r)
FILE *stream;
avltree_tree *t;
avltree_node *r;
{
    if (!r)
        return;
    if (r != t->root)
        fprintf(stream, "%s", t->stprint.separator);
    if (t->stprint.print_fn)
        t->stprint.print_fn(stream, r->key, r->value);
    avl_prefix(stream, t, r->child[0]);
    avl_prefix(stream, t, r->child[1]);
}

void avl_dump_prefix(stream, t, r)
FILE *stream;
avltree_tree *t;
avltree_node *r;
{
    if (!r)
        return;
    if (r != t->root)
        putc('\n', stream);
    fprintf(stream, "%p ", r);
    if (t->stprint.print_fn)
        t->stprint.print_fn(stream, r->key, r->value);
    fprintf(stream, " [%d] (%p, %p) ^%p", r->bf, r->child[0], r->child[1], r->parent);
    avl_dump_prefix(stream, t, r->child[0]);
    avl_dump_prefix(stream, t, r->child[1]);
}

void avl_posfix(stream, t, r)
FILE *stream;
avltree_tree *t;
avltree_node *r;
{
    if (!r)
        return;
    avl_posfix(stream, t, r->child[0]);
    avl_posfix(stream, t, r->child[1]);
    if (t->stprint.print_fn)
        t->stprint.print_fn(stream, r->key, r->value);
    if (r != t->root)
        fprintf(stream, "%s", t->stprint.separator);
}

void avl_dump_posfix(stream, t, r)
FILE *stream;
avltree_tree *t;
avltree_node *r;
{
    if (!r)
        return;
    avl_dump_posfix(stream, t, r->child[0]);
    avl_dump_posfix(stream, t, r->child[1]);
    fprintf(stream, "%p ", r);
    if (t->stprint.print_fn)
        t->stprint.print_fn(stream, r->key, r->value);
    fprintf(stream, " [%d] (%p, %p) ^%p", r->bf, r->child[0], r->child[1], r->parent);
    if (r != t->root)
        putc('\n', stream);
}

/* Assumes that arr has sufficient capacity. */
void avl_keys_to_array(t, r, arr, size, nmemb, capacity)
avltree_tree *t;
avltree_node *r;
unsigned char *arr;
size_t size;
int *nmemb;
{
    if (!r)
        return;
    avl_keys_to_array(t, r->child[0], arr, size, nmemb, capacity);
    assert(*nmemb < capacity);
    memcpy(&arr[size**nmemb], r->key, size);
    ++*nmemb;
    avl_keys_to_array(t, r->child[1], arr, size, nmemb, capacity);
}

/* Debug functions: */

avl_height(stream, t, r)
FILE *stream;
avltree_tree *t;
avltree_node *r;
{
    int lh, rh;
    if (!r)
        return 0;
    lh = avl_height(stream, t, r->child[0])+1;
    rh = avl_height(stream, t, r->child[1])+1;
    if (r->bf != rh - lh || abs(r->bf) >= 2) {
        fprintf(stream, "Node ");
        if (t->stprint.print_fn)
            t->stprint.print_fn(stream, r->key, r->value);
        else
            fprintf(stream, "%p", r);
        fprintf(stream, " rh: %d lh: %d, BF -> %d\n", rh, lh, r->bf);
        exit(EXIT_FAILURE);
    }
    return rh > lh? rh : lh;
}
