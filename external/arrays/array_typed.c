/*
    array_typed.c - v2.0.0
    Typed growable array implementation in C.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    array_typed.c is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef ARRAYTYPED_H
#define ARRAYTYPED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define arraytyped_allocate_ptr(TYPENAME, ARR, CAP)\
    do {\
        (ARR)->nmemb = 0;\
        (ARR)->capacity = CAP;\
        (ARR)->base = malloc(sizeof(TYPENAME) * (CAP));\
        if (!(ARR)->base) { \
            fputs("[arraytyped_allocate_ptr] Not enough memory.\n", stderr); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)
#define arraytyped_allocate(TYPENAME, ARR, CAP) arraytyped_allocate_ptr(TYPENAME, &(ARR), CAP)

#define ARRAYTYPED_LAST_SPACE_PTR(ARR) ((ARR)->base + (ARR)->nmemb)
#define ARRAYTYPED_LAST_SPACE(ARR) ARRAYTYPED_LAST_SPACE_PTR(&(ARR))

#define ARRAYTYPED_AT_PTR(ARR, I) ((ARR)->base + (I))
#define ARRAYTYPED_AT(ARR, I) ARRAYTYPED_AT_PTR(&(ARR), I)

/* Be careful if you use nmemb as idx, because nmemb is incremented before
 * doing the appending. It is better call arraytyped_append_to_end if the item
 * needs to be placed in last. */
#define arraytyped_append_to_idx_ptr(TYPENAME, ARR, I, ITEM) \
    do {\
        (ARR)->nmemb++;\
        arraytyped_expand_##TYPENAME(ARR);\
        memcpy((ARR)->base + (I), ITEM, sizeof(TYPENAME));\
    } while (0)
#define arraytyped_append_to_idx(TYPENAME, ARR, I, ITEM) arraytyped_append_to_idx_ptr(TYPENAME, &(ARR), I, ITEM)

#define arraytyped_append_to_end_ptr(TYPENAME, ARR, ITEM) arraytyped_append_to_idx_ptr(TYPENAME, ARR, (ARR)->nmemb-1, ITEM)
#define arraytyped_append_to_end(TYPENAME, ARR, ITEM) arraytyped_append_to_idx_ptr(TYPENAME, &(ARR), (ARR).nmemb-1, ITEM)

#define arraytyped_qsort_ptr(TYPENAME, ARR, CMP) \
    qsort((ARR)->base, (ARR)->nmemb, sizeof(TYPENAME), CMP);
#define arraytyped_qsort(TYPENAME, ARR, CMP) arraytyped_qsort_ptr(TYPENAME, &(ARR), CMP)

#define ARRAYTYPED_GENERATE(TYPENAME, FAC, INC)\
\
typedef struct {\
    TYPENAME *base;\
    size_t nmemb, capacity;\
} arraytyped_array_##TYPENAME;\
\
void arraytyped_expand_##TYPENAME(arr)\
arraytyped_array_##TYPENAME *arr;\
{\
    TYPENAME *tmp;\
    \
    if (arr->nmemb == arr->capacity) {\
        arr->capacity = arr->capacity * (FAC) + (INC);\
        if ((tmp = realloc(arr->base, sizeof(TYPENAME) * arr->capacity)))\
            arr->base = tmp;\
        else {\
            fprintf(stderr, "[%s] Realloc fail.\n", __func__);\
            exit(EXIT_FAILURE);\
        }\
    }\
}\
\
void arraytyped_move_##TYPENAME(end, begin, offset)\
TYPENAME *end, *begin;\
size_t offset;\
{\
    for (; end >= begin; end--)\
        memcpy(end+offset, end, sizeof(TYPENAME));\
}

#define ARRAYTYPED_GENERATE_ORDERED_INSERT(TYPENAME, INPLACE, CMP)\
size_t arraytyped_upper_bound_##TYPENAME(arr, item, begin, end)\
TYPENAME *arr, *item;\
{\
    size_t idx;\
    int cmp;\
    \
    idx = (begin + end)/2;\
    while ((cmp = CMP(item, arr+idx))) {\
        if (cmp < 0) {\
            if (idx == begin)\
                return idx;\
            end = --idx;\
        } else {\
            if (idx == end)\
                return idx+1;\
            begin = ++idx;\
        }\
        idx = (begin + end)/2;\
    }\
    return idx;\
}\
\
arraytyped_insert_aorder_##TYPENAME(arr, item, idx)\
arraytyped_array_##TYPENAME *arr;\
TYPENAME *item;\
size_t *idx;\
{\
    if (!arr->nmemb) {\
        *idx = 0;\
        arraytyped_append_to_idx_ptr(TYPENAME, arr, *idx, item);\
        return 1;\
    }\
    if ((*idx = arraytyped_upper_bound_##TYPENAME(arr->base, item, 0, arr->nmemb-1)) == arr->nmemb) {\
        arraytyped_append_to_idx_ptr(TYPENAME, arr, *idx, item);\
        return 1;\
    }\
    if (CMP(arr->base + *idx, item)) {\
        arr->nmemb++;\
        arraytyped_expand_##TYPENAME(arr);\
        arraytyped_move_##TYPENAME(arr->base + arr->nmemb-2, arr->base + *idx, 1);\
        memcpy(arr->base + *idx, item, sizeof(TYPENAME));\
        return 1;\
    }\
    if (INPLACE)\
        memcpy(arr->base + *idx, item, sizeof(TYPENAME));\
    return 0;\
}\
\
TYPENAME *arraytyped_find_##TYPENAME(arr, item)\
arraytyped_array_##TYPENAME *arr;\
TYPENAME *item;\
{\
    size_t idx;\
    TYPENAME *ptr;\
    \
    if (!arr->nmemb)\
        return NULL;\
    if ((idx =\
         arraytyped_upper_bound_##TYPENAME(arr->base, item, 0, arr->nmemb-1))\
          == arr->nmemb)\
        return NULL;\
    \
    ptr = arr->base + idx;\
    if (CMP(ptr, item))\
        return NULL;\
    return ptr;\
}

#endif
