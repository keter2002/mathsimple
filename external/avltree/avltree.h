/*
    avltree.h - v2.0.0
    AVL tree implementation in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v2.0.0  avl_diff(), avl_copy_keys(), flags parameter in avl_remove,
                stream pointer parameter and separator string in printing
                routines.
        v1.1.0  Functions to free only avltree_node struct
        v1.0.0  First version

    avltree.h is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdio.h>

#define AVLTREE_FREE_NONE  00
#define AVLTREE_FREE_KEY   01
#define AVLTREE_FREE_VALUE 02
#define AVLTREE_FREE_BOTH  03

typedef struct avltree_node {
    short bf;
    struct avltree_node *parent, *child[2];
    void *key, *value;
    unsigned char has_value;
} avltree_node;

typedef struct {
    avltree_node *root;
    int nmemb;
    int (*compar)(const void *, const void *);
    struct {
        /* stream, key and value. */
        void (*print_fn)(FILE*, void *, void *);
        char *separator;
    } stprint;
    /* If true, the insertion replaces values with the same key */
    unsigned char inplace;
} avltree_tree;


#define avltree_create(T, INPLACE, CMP_FN, PRINT_FN, SEPARATOR) \
    do { \
         T.root = NULL; \
         T.nmemb = 0; \
         T.inplace = INPLACE; \
         T.compar = CMP_FN; \
         T.stprint.print_fn = PRINT_FN; \
         T.stprint.separator = SEPARATOR; \
    } while (0)

void avl_destroy(avltree_tree *t, avltree_node *r);
#define avltree_destroy(T) \
    do { \
        if (T.nmemb) { \
            avl_destroy(&T, T.root); \
            T.nmemb = 0; \
            T.root = NULL; \
        } \
    } while (0)
#define avltree_destroy_ptr(T) \
    do { \
        if (T->nmemb) { \
            avl_destroy(T, T->root); \
            T->nmemb = 0; \
            T->root = NULL; \
        } \
    } while (0)

/* Does not release memory in key and value. */
void avl_empty(avltree_tree *t, avltree_node *r);
#define avltree_empty(T) \
    avl_empty(&(T), (T).root)
#define avltree_empty_ptr(T) \
    avl_empty(T, (T)->root)

void avl_copy_keys(avltree_tree *td, avltree_tree *ts, avltree_node *r);
#define avltree_copy_keys(TD, TS) \
    avl_copy_keys(&(TD), &(TS), (TS).root)
#define avltree_copy_keys_ptr(TD, TS) \
    avl_copy_keys(TD, TS, (TS)->root)

void avl_diff(avltree_tree *td, avltree_tree *ts, avltree_node *r, unsigned char flags);
#define avltree_diff(TD, TS, FLAGS) \
    avl_diff(&(TD), &(TS), (TS).root, FLAGS)
#define avltree_diff_ptr(TD, TS, FLAGS) \
    avl_diff(TD, TS, (TS)->root, FLAGS)

avltree_node *avl_find_max(avltree_tree *t, avltree_node *r);
#define avltree_find_max(T) \
    avl_find_max(&T, T.root)
avltree_node *avl_find_min(avltree_tree *t, avltree_node *r);
#define avltree_find_min(T) \
    avl_find_min(&T, T.root)

avltree_node *avl_find_node(avltree_tree *t, avltree_node *r, void *key, avltree_node **parent);
#define avltree_find_node(T, KEY) \
    avl_find_node(&T, T.root, KEY, NULL)
#define avltree_find_node_ptr(T, KEY) \
    avl_find_node(T, T->root, KEY, NULL)

avltree_node *avltree_insert(avltree_tree *t, void *key, void *value);
#define avltree_insert_key(T, KEY) \
    avltree_insert(&T, KEY, NULL)
#define avltree_insert_key_ptr(T, KEY) \
    avltree_insert(T, KEY, NULL)

void avl_remove(avltree_tree *t, avltree_node *z, unsigned char flags);

int avltree_remove(avltree_tree *t, void *key, unsigned char flags);
#define avltree_remove_node(T, KEY, FLAGS) \
    avltree_remove(&T, KEY, FLAGS)
#define avltree_remove_node_ptr(T, KEY, FLAGS) \
    avltree_remove(T, KEY, FLAGS)

int avl_height(FILE *stream, avltree_tree *t, avltree_node *r);
#define avltree_height(STREAM, T) \
    avl_height(STREAM, &T, T.root)

void avl_infix(FILE *stream, avltree_tree *t, avltree_node *r, avltree_node *last);
#define avltree_infix(STREAM, T) \
    avl_infix(STREAM, &T, T.root, avl_find_max(&T, T.root))

void avl_prefix(FILE *stream, avltree_tree *t, avltree_node *r);
#define avltree_prefix(STREAM, T) \
    avl_prefix(STREAM, &T, T.root)
#define avltree_prefix_ptr(STREAM, T) \
    avl_prefix(STREAM, T, T->root)

void avl_posfix(FILE *stream, avltree_tree *t, avltree_node *r);
#define avltree_posfix(STREAM, T) \
    avl_posfix(STREAM, &T, T.root)

void avl_keys_to_array(avltree_tree *t, avltree_node *r, unsigned char *arr, size_t size, int *nmemb, int capacity);
#define avltree_keys_to_array(T, ARR, SIZE, NMEMB, CAP) \
    avl_keys_to_array(&T, T.root, ARR, SIZE, NMEMB, CAP)

#endif
