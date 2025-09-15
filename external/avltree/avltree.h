/*
    avltree.h - v1.0.0
    AVL tree implementation in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

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
    void (*print_node)(FILE*, void *, void *);
    /* If true, the insertion replaces values with the same key */
    unsigned char inplace;
} avltree_tree;

#define avltree_create(T, INPLACE, CMP_FN, PRINT_FN) \
    do { \
         T.root = NULL; \
         T.nmemb = 0; \
         T.inplace = INPLACE; \
         T.compar = CMP_FN; \
         T.print_node = PRINT_FN; \
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

void avl_remove(avltree_tree *t, avltree_node *z);

int avltree_remove(avltree_tree *t, void *key);
#define avltree_remove_node(T, KEY) \
    avltree_remove(&T, KEY)
#define avltree_remove_node_ptr(T, KEY) \
    avltree_remove(T, KEY)

int avl_height(avltree_tree *t, avltree_node *r);
#define avltree_height(T) \
    avl_height(&T, T.root)

void avl_infix(avltree_tree *t, avltree_node *r, avltree_node *last);
#define avltree_infix(T) \
    avl_infix(&T, T.root, avltree_find_max(&T, T.root))

void avl_prefix(avltree_tree *t, avltree_node *r);
#define avltree_prefix(T) \
    avl_prefix(&T, T.root)
#define avltree_prefix_ptr(T) \
    avl_prefix(T, T->root)

void avl_posfix(avltree_tree *t, avltree_node *r);
#define avltree_posfix(T) \
    avl_posfix(&T, T.root)

void avl_keys_to_array(avltree_tree *t, avltree_node *r, unsigned char *arr, size_t size, int *nmemb, int capacity);
#define avltree_keys_to_array(T, ARR, SIZE, NMEMB, CAP) \
    avl_keys_to_array(&T, T.root, ARR, SIZE, NMEMB, CAP)

#endif
