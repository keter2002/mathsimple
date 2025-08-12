/*
    AVL tree implementation in C.
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

#ifndef H_AVLTREE
#define H_AVLTREE
#include <stdio.h>

typedef struct avl_node {
    short bf;
    struct avl_node *parent, *child[2];
    void *key, *value;
    unsigned char has_value;
} avl_node;

typedef struct {
    struct avl_node *root;
    int nmemb;
    int (*compar)(const void *, const void *);
    void (*print_node)(FILE*, void *, void *);
    /* If true, the insertion replaces values with the same key */
    unsigned char inplace;
} avl_tree;

#define create_avltree(T, INPLACE, CMP_FN, PRINT_FN) \
    do { \
         T.root = NULL; \
         T.nmemb = 0; \
         T.inplace = INPLACE; \
         T.compar = CMP_FN; \
         T.print_node = PRINT_FN; \
    } while (0)

void destroy_avl(avl_tree *t, avl_node *r);
#define destroy_avltree(T) \
    do { \
        if (T.nmemb) { \
            destroy_avl(&T, T.root); \
            T.nmemb = 0; \
            T.root = NULL; \
        } \
    } while (0)
#define destroy_avltree_ptr(T) \
    do { \
        if (T->nmemb) { \
            destroy_avl(T, T->root); \
            T->nmemb = 0; \
            T->root = NULL; \
        } \
    } while (0)

avl_node *find_max_avl(avl_tree *t, avl_node *r);
#define find_max_avltree(T) \
    find_max_avl(&T, T.root)
avl_node *find_min_avl(avl_tree *t, avl_node *r);
#define find_min_avltree(T) \
    find_min_avl(&T, T.root)

avl_node *find_node_avl(avl_tree *t, avl_node *r, void *key, avl_node **parent);
#define find_node_avltree(T, KEY) \
    find_node_avl(&T, T.root, KEY, NULL)
#define find_node_avltree_ptr(T, KEY) \
    find_node_avl(T, T->root, KEY, NULL)

avl_node *insert_avltree(avl_tree *t, void *key, void *value);
#define insert_key_avltree(T, KEY) \
    insert_avltree(&T, KEY, NULL)
#define insert_key_avltree_ptr(T, KEY) \
    insert_avltree(T, KEY, NULL)

void remove_avl(avl_tree *t, avl_node *z);

int remove_avltree(avl_tree *t, void *key);
#define remove_node_avltree(T, KEY) \
    remove_avltree(&T, KEY)
#define remove_node_avltree_ptr(T, KEY) \
    remove_avltree(T, KEY)

int height_avl(avl_tree *t, avl_node *r);
#define height_avltree(T) \
    height_avl(&T, T.root)

void infix_avl(avl_tree *t, avl_node *r, avl_node *last);
#define infix_avltree(T) \
    infix_avl(&T, T.root, find_max_avltree(&T, T.root))

void prefix_avl(avl_tree *t, avl_node *r);
#define prefix_avltree(T) \
    prefix_avl(&T, T.root)
#define prefix_avltree_ptr(T) \
    prefix_avl(T, T->root)

void posfix_avl(avl_tree *t, avl_node *r);
#define posfix_avltree(T) \
    posfix_avl(&T, T.root)

void keys_to_array_avl(avl_tree *t, avl_node *r, unsigned char *arr, size_t size, int *nmemb, int capacity);
#define keys_to_array_avltree(T, ARR, SIZE, NMEMB, CAP) \
    keys_to_array_avl(&T, T.root, ARR, SIZE, NMEMB, CAP)

#endif
