/*
    Typed growable array implementation in C.
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

#ifndef H_ARRAYTYPED
#define H_ARRAYTYPED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define allocate_ptr_arraytyped(TYPENAME, ARR, CAP)\
    do {\
	    (ARR)->nmemb = 0;\
	    (ARR)->capacity = CAP;\
	    (ARR)->base = malloc(sizeof(TYPENAME) * (CAP));\
	    if (!(ARR)->base) { \
	    	fputs("[allocate_ptr_arraytyped] Not enough memory.\n", stderr); \
	    	exit(1); \
	    } \
    } while (0)
#define allocate_arraytyped(TYPENAME, ARR, CAP) allocate_ptr_arraytyped(TYPENAME, &(ARR), CAP)

#define LAST_SPACE_PTR_ARRAYTYPED(ARR) ((ARR)->base + (ARR)->nmemb)
#define LAST_SPACE_ARRAYTYPED(ARR) LAST_SPACE_PTR_ARRAYTYPED(&(ARR))

#define AT_PTR_ARRAYTYPED(ARR, I) ((ARR)->base + (I))
#define AT_ARRAYTYPED(ARR, I) AT_PTR_ARRAYTYPED(&(ARR), I)

#define APPEND_TO_IDX_PTR_ARRAYTYPED(TYPENAME, ARR, I, ITEM) \
    do {\
        (ARR)->nmemb++;\
        expand_arraytyped_##TYPENAME(ARR);\
        memcpy((ARR)->base + (I), ITEM, sizeof(TYPENAME));\
    } while (0)
#define APPEND_TO_IDX_ARRAYTYPED(TYPENAME, ARR, I, ITEM) APPEND_TO_IDX_PTR_ARRAYTYPED(TYPENAME, &(ARR), I, ITEM)

#define GENERATE_ARRAYTYPED(TYPENAME, FAC, INC)\
\
typedef struct {\
	TYPENAME *base;\
	size_t nmemb, capacity;\
} array_arraytyped_##TYPENAME;\
\
void expand_arraytyped_##TYPENAME(arr)\
array_arraytyped_##TYPENAME *arr;\
{\
    TYPENAME *tmp;\
    \
    if (arr->nmemb == arr->capacity) {\
        arr->capacity = arr->capacity * (FAC) + (INC);\
    	if ((tmp = realloc(arr->base, sizeof(TYPENAME) * arr->capacity)))\
    		arr->base = tmp;\
    	else {\
    		fprintf(stderr, "[%s] Realloc fail.\n", __func__);\
    		exit(1);\
    	}\
    }\
}\
\
void move_arraytyped_##TYPENAME(end, begin, offset)\
TYPENAME *end, *begin;\
size_t offset;\
{\
    for (; end >= begin; end--)\
        memcpy(end+offset, end, sizeof(TYPENAME));\
}

#define GENERATE_ORDERED_INSERT_ARRAYTYPED(TYPENAME, INPLACE, CMP)\
size_t upper_bound_arraytyped_##TYPENAME(arr, item, begin, end)\
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
insert_aorder_arraytyped_##TYPENAME(arr, item, idx)\
array_arraytyped_##TYPENAME *arr;\
TYPENAME *item;\
size_t *idx;\
{\
    if (!arr->nmemb) {\
        *idx = 0;\
        APPEND_TO_IDX_PTR_ARRAYTYPED(TYPENAME, arr, *idx, item);\
        return 1;\
    }\
    if ((*idx = upper_bound_arraytyped_##TYPENAME(arr->base, item, 0, arr->nmemb-1)) == arr->nmemb) {\
        APPEND_TO_IDX_PTR_ARRAYTYPED(TYPENAME, arr, *idx, item);\
        return 1;\
    }\
    if (CMP(arr->base + *idx, item)) {\
        arr->nmemb++;\
        expand_arraytyped_##TYPENAME(arr);\
        move_arraytyped_##TYPENAME(arr->base + arr->nmemb-2, arr->base + *idx, 1);\
        memcpy(arr->base + *idx, item, sizeof(TYPENAME));\
        return 1;\
    }\
    if (INPLACE)\
        memcpy(arr->base + *idx, item, sizeof(TYPENAME));\
}

#define GENERATE_SORT_ARRAYTYPED(TYPENAME, CMP)\
static void swap_arraytyped_##TYPENAME(x, y)\
TYPENAME *x, *y;\
{\
    TYPENAME aux;\
\
    memcpy(&aux, x, sizeof(TYPENAME));\
    memcpy(x, y, sizeof(TYPENAME));\
    memcpy(y, &aux, sizeof(TYPENAME));\
}\
\
void quick_sort_arraytyped_##TYPENAME(arr, begin, end)\
TYPENAME arr[];\
{\
    TYPENAME pivot;\
    int i, j;\
\
    i = begin;\
    j = end - 1;\
    memcpy(&pivot, arr + ((begin + end) / 2), sizeof(TYPENAME));\
    while (i <= j){\
        for (;CMP(arr + i, &pivot) < 0 && i < end; ++i);\
        for (;CMP(arr + j, &pivot) > 0 && j > begin; --j);\
\
        if (i <= j){\
            swap_arraytyped_##TYPENAME(arr + i, arr + j);\
            --j;\
            ++i;\
        }\
    }\
\
    if (j > begin)\
        quick_sort_arraytyped_##TYPENAME(arr, begin, j + 1);\
    if (i < end)\
        quick_sort_arraytyped_##TYPENAME(arr, i, end);\
}

#endif
