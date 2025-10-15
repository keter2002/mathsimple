/* Código reescrito por João Manica  <joaoedisonmanica@gmail.com>. */
/* Autor original foi perdido. Código originalmente similar ao encontrado em:
 * https://github.com/MillenniumDB/MillenniumDB/blob/dev/src/misc/min_max_heap.h*/

#ifndef MINMAXH_H
#define MINMAXH_H

#include <math.h>
#include <string.h>


#define minmaxh_allocate_ptr(TYPENAME, ARR, CAP)\
    do {\
        (ARR)->nmemb = 0;\
        (ARR)->capacity = CAP;\
        (ARR)->base = malloc(sizeof(TYPENAME) * (CAP));\
        if (!(ARR)->base) { \
            fputs("[allocate_ptr_arraytyped] Not enough memory.\n", stderr); \
            exit(1); \
        } \
    } while (0)
#define minmaxh_allocate(TYPENAME, ARR, CAP) minmaxh_allocate_ptr(TYPENAME, &(ARR), CAP)

#define MINMAXH_MIN_LEVEL(I) \
    ((int)log2((I)+1)%2==0)

#define MINMAXH_PARENT(I) \
    (((I)-1 < 0)? (I)-1 : ((I)-1)>>1)

#define MINMAXH_HAS_GRANDPARENT(I) \
    (((I)-3)>>2 >= 0? 1 : 0)

#define MINMAXH_GRANDPARENT(I) \
    (((I)-3)>>2)

#define MINMAXH_HAS_CHILDREN(I, NMEMB) \
    ((I)<<1 < (NMEMB)? 1 : 0)

#define MINMAXH_LEFT_CHILD(I, NMEMB) \
    (((I)<<1)+1 < (NMEMB)? ((I)<<1)+1 : -1)

#define MINMAXH_RIGHT_CHILD(I, NMEMB) \
    (((I)<<1)+2 < (NMEMB)? ((I)<<1)+2 : -1)

#define MINMAXH_GENERATE(TYPENAME, CMP)\
\
typedef struct {\
    TYPENAME *base;\
    int nmemb, capacity;\
} minmaxh_heap_##TYPENAME;\
\
void minmaxh_insert_##TYPENAME(minmaxh_heap_##TYPENAME *h, TYPENAME *value);\
void minmaxh_remove_min_##TYPENAME(minmaxh_heap_##TYPENAME *h, TYPENAME *value);\
void minmaxh_remove_max_##TYPENAME(minmaxh_heap_##TYPENAME *h, TYPENAME *value);\
\
static void minmaxh_push_down_##TYPENAME(TYPENAME *h, int i, int nmemb);\
static void minmaxh_push_down_min_##TYPENAME(TYPENAME *h, int i, int nmemb);\
static void minmaxh_push_down_max_##TYPENAME(TYPENAME *h, int i, int nmemb);\
static int minmaxh_smallest_##TYPENAME(TYPENAME *h, int i, int *g, int nmemb);\
static int minmaxh_largest_##TYPENAME(TYPENAME *h, int i, int *g, int nmemb);\
static void minmaxh_child_##TYPENAME(TYPENAME *h, int c, int *m, int *g, int cmp, int nmemb);\
static void minmaxh_push_up_##TYPENAME(TYPENAME *h, int i);\
static void minmaxh_push_up_min_##TYPENAME(TYPENAME *h, int i);\
static void minmaxh_push_up_max_##TYPENAME(TYPENAME *h, int i);\
static void minmaxh_swap_##TYPENAME(TYPENAME *x, TYPENAME *y);\
\
void minmaxh_insert_##TYPENAME(h, value)\
minmaxh_heap_##TYPENAME *h;\
TYPENAME *value;\
{\
    assert(h->nmemb < h->capacity);\
    memcpy(&h->base[h->nmemb], value, sizeof(TYPENAME));\
    minmaxh_push_up_##TYPENAME(h->base, h->nmemb);\
    h->nmemb++;\
}\
\
static void minmaxh_push_up_##TYPENAME(h, i)\
TYPENAME *h;\
{\
    int parent;\
\
    if (i) {\
        parent = MINMAXH_PARENT(i);\
        if (MINMAXH_MIN_LEVEL(i)) {\
            if (CMP(&h[i], &h[parent]) > 0) {\
                minmaxh_swap_##TYPENAME(&h[i], &h[parent]);\
                minmaxh_push_up_max_##TYPENAME(h, parent);\
            } else\
                minmaxh_push_up_min_##TYPENAME(h, i);\
        } else {\
            if (CMP(&h[i], &h[parent]) < 0) {\
                minmaxh_swap_##TYPENAME(&h[i], &h[parent]);\
                minmaxh_push_up_min_##TYPENAME(h, parent);\
            } else\
                minmaxh_push_up_max_##TYPENAME(h, i);\
        }\
    }\
}\
\
static void minmaxh_push_up_min_##TYPENAME(h, i)\
TYPENAME *h;\
{\
    int grandparent;\
\
    while (MINMAXH_HAS_GRANDPARENT(i) &&\
           CMP(&h[i], &h[(grandparent = MINMAXH_GRANDPARENT(i))]) < 0) {\
        minmaxh_swap_##TYPENAME(&h[i], &h[grandparent]);\
        i = grandparent;\
    }\
}\
\
static void minmaxh_push_up_max_##TYPENAME(h, i)\
TYPENAME *h;\
{\
    int grandparent;\
\
    while (MINMAXH_HAS_GRANDPARENT(i) &&\
           CMP(&h[i], &h[(grandparent = MINMAXH_GRANDPARENT(i))]) > 0) {\
        minmaxh_swap_##TYPENAME(&h[i], &h[grandparent]);\
        i = grandparent;\
    }\
}\
\
void minmaxh_remove_min_##TYPENAME(h, value)\
minmaxh_heap_##TYPENAME *h;\
TYPENAME *value;\
{\
    if (h->nmemb) {\
        memcpy(value, h->base, sizeof(TYPENAME));\
        memcpy(h->base, &h->base[--h->nmemb], sizeof(TYPENAME));\
        minmaxh_push_down_##TYPENAME(h->base, 0, h->nmemb);\
    }\
}\
\
void minmaxh_remove_max_##TYPENAME(h, value)\
minmaxh_heap_##TYPENAME *h;\
TYPENAME *value;\
{\
    int i;\
\
    if (h->nmemb) {\
        if (h->nmemb <= 2)\
            i = h->nmemb-1;\
        else\
            i = CMP(&h->base[1], &h->base[2]) > 0? 1 : 2;\
        memcpy(value, &h->base[i], sizeof(TYPENAME));\
        memcpy(&h->base[i], &h->base[--h->nmemb], sizeof(TYPENAME));\
        minmaxh_push_down_##TYPENAME(h->base, i, h->nmemb);\
    }\
}\
\
static void minmaxh_push_down_##TYPENAME(h, i, nmemb)\
TYPENAME *h;\
{\
    if (MINMAXH_MIN_LEVEL(i))\
        minmaxh_push_down_min_##TYPENAME(h, i, nmemb);\
    else\
        minmaxh_push_down_max_##TYPENAME(h, i, nmemb);\
}\
\
static void minmaxh_swap_##TYPENAME(x, y)\
TYPENAME *x, *y;\
{\
    TYPENAME aux;\
\
    memcpy(&aux, x, sizeof(TYPENAME));\
    memcpy(x, y, sizeof(TYPENAME));\
    memcpy(y, &aux, sizeof(TYPENAME));\
}\
\
static void minmaxh_push_down_min_##TYPENAME(h, i, nmemb)\
TYPENAME *h;\
{\
    int m, g, parent;\
\
    while (MINMAXH_HAS_CHILDREN(i, nmemb)) {\
        g = 0;\
        m = minmaxh_smallest_##TYPENAME(h, i, &g, nmemb);\
        if (m >= 0) {\
            if (g && (CMP(&h[m], &h[i]) < 0)) {\
                minmaxh_swap_##TYPENAME(&h[m], &h[i]);\
                if ((parent = MINMAXH_PARENT(m)) >= 0)\
                    if (CMP(&h[m], &h[parent]) > 0)\
                        minmaxh_swap_##TYPENAME(&h[m], &h[parent]);\
                i = m;\
                continue;\
            } else if (CMP(&h[m], &h[i]) < 0)\
                minmaxh_swap_##TYPENAME(&h[m], &h[i]);\
        }\
        break;\
    }\
}\
\
static void minmaxh_push_down_max_##TYPENAME(h, i, nmemb)\
TYPENAME *h;\
{\
    int m, g, parent;\
\
    while (MINMAXH_HAS_CHILDREN(i, nmemb)) {\
        g = 0;\
        m = minmaxh_largest_##TYPENAME(h, i, &g, nmemb);\
        if (m >= 0) {\
            if (g && (CMP(&h[m], &h[i]) > 0)) {\
                minmaxh_swap_##TYPENAME(&h[m], &h[i]);\
                if ((parent = MINMAXH_PARENT(m)) >= 0)\
                    if (CMP(&h[m], &h[parent]) < 0)\
                        minmaxh_swap_##TYPENAME(&h[m], &h[parent]);\
                i = m;\
                continue;\
            } else if (CMP(&h[m], &h[i]) > 0)\
                minmaxh_swap_##TYPENAME(&h[m], &h[i]);\
        }\
        break;\
    }\
}\
\
static int minmaxh_smallest_##TYPENAME(h, i, g, nmemb)\
TYPENAME *h;\
int *g;\
{\
    int left, right, m;\
\
    m = left = MINMAXH_LEFT_CHILD(i, nmemb);\
    if (m < 0)\
        return m;\
    right = MINMAXH_RIGHT_CHILD(i, nmemb);\
    if (m+1 < nmemb && CMP(&h[m+1], &h[m]) < 0)\
        m++;\
    minmaxh_child_##TYPENAME(h, left, &m, g, -1, nmemb);\
    minmaxh_child_##TYPENAME(h, right, &m, g, -1, nmemb);\
    return m;\
}\
\
static int minmaxh_largest_##TYPENAME(h, i, g, nmemb)\
TYPENAME *h;\
int *g;\
{\
    int left, right, m;\
\
    m = left = MINMAXH_LEFT_CHILD(i, nmemb);\
    if (m < 0)\
        return m;\
    right = MINMAXH_RIGHT_CHILD(i, nmemb);\
    if (m+1 < nmemb && CMP(&h[m+1], &h[m]) > 0)\
        m++;\
    minmaxh_child_##TYPENAME(h, left, &m, g, 1, nmemb);\
    minmaxh_child_##TYPENAME(h, right, &m, g, 1, nmemb);\
    return m;\
}\
\
static void minmaxh_child_##TYPENAME(h, c, m, g, cmp, nmemb)\
TYPENAME *h;\
int *m, *g;\
{\
    int n, k, left;\
\
    if ((left = MINMAXH_LEFT_CHILD(c, nmemb)) >= 0)\
        for (n=2, k=left; k < nmemb && n--; k++)\
            if (cmp * (CMP(&h[k], &h[*m])) > 0) {\
                *g = 1;\
                *m = k;\
            }\
}
#endif
