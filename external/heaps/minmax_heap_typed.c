/* Código reescrito por João Manica  <joaoedisonmanica@gmail.com>. */
/* Autor original foi perdido. Código originalmente similar ao encontrado em:
 * https://github.com/MillenniumDB/MillenniumDB/blob/dev/src/misc/min_max_heap.h*/

#ifndef H_MINMAXH
#define H_MINMAXH

#include <math.h>
#include <string.h>


#define allocate_ptr_minmaxh(TYPENAME, ARR, CAP)\
    do {\
	    (ARR)->nmemb = 0;\
	    (ARR)->capacity = CAP;\
	    (ARR)->base = malloc(sizeof(TYPENAME) * (CAP));\
	    if (!(ARR)->base) { \
	    	fputs("[allocate_ptr_arraytyped] Not enough memory.\n", stderr); \
	    	exit(1); \
	    } \
    } while (0)
#define allocate_minmaxh(TYPENAME, ARR, CAP) allocate_ptr_minmaxh(TYPENAME, &(ARR), CAP)

#define MIN_LEVEL_MINMAXH(I) \
    ((int)log2((I)+1)%2==0)

#define PARENT_MINMAXH(I) \
    (((I)-1 < 0)? (I)-1 : ((I)-1)>>1)

#define HAS_GRANDPARENT_MINMAXH(I) \
    (((I)-3)>>2 >= 0? 1 : 0)

#define GRANDPARENT_MINMAXH(I) \
    (((I)-3)>>2)

#define HAS_CHILDREN_MINMAXH(I, NMEMB) \
    ((I)<<1 < (NMEMB)? 1 : 0)

#define LEFT_CHILD_MINMAXH(I, NMEMB) \
    (((I)<<1)+1 < (NMEMB)? ((I)<<1)+1 : -1)

#define RIGHT_CHILD_MINMAXH(I, NMEMB) \
    (((I)<<1)+2 < (NMEMB)? ((I)<<1)+2 : -1)

#define GENERATE_MINMAXH(TYPENAME, CMP)\
\
typedef struct {\
    TYPENAME *base;\
    int nmemb, capacity;\
} heap_minmaxh_##TYPENAME;\
\
void insert_minmaxh_##TYPENAME(heap_minmaxh_##TYPENAME *h, TYPENAME *value);\
void remove_min_minmaxh_##TYPENAME(heap_minmaxh_##TYPENAME *h, TYPENAME *value);\
void remove_max_minmaxh_##TYPENAME(heap_minmaxh_##TYPENAME *h, TYPENAME *value);\
\
static void push_down_minmaxh_##TYPENAME(TYPENAME *h, int i, int nmemb);\
static void push_down_min_minmaxh_##TYPENAME(TYPENAME *h, int i, int nmemb);\
static void push_down_max_minmaxh_##TYPENAME(TYPENAME *h, int i, int nmemb);\
static int smallest_minmaxh_##TYPENAME(TYPENAME *h, int i, int *g, int nmemb);\
static int largest_minmaxh_##TYPENAME(TYPENAME *h, int i, int *g, int nmemb);\
static void child_minmaxh_##TYPENAME(TYPENAME *h, int c, int *m, int *g, int cmp, int nmemb);\
static void push_up_minmaxh_##TYPENAME(TYPENAME *h, int i);\
static void push_up_min_minmaxh_##TYPENAME(TYPENAME *h, int i);\
static void push_up_max_minmaxh_##TYPENAME(TYPENAME *h, int i);\
static void swap_minmaxh_##TYPENAME(TYPENAME *x, TYPENAME *y);\
\
void insert_minmaxh_##TYPENAME(h, value)\
heap_minmaxh_##TYPENAME *h;\
TYPENAME *value;\
{\
    assert(h->nmemb < h->capacity);\
    memcpy(&h->base[h->nmemb], value, sizeof(TYPENAME));\
    push_up_minmaxh_##TYPENAME(h->base, h->nmemb);\
    h->nmemb++;\
}\
\
static void push_up_minmaxh_##TYPENAME(h, i)\
TYPENAME *h;\
{\
    int parent;\
\
    if (i) {\
        parent = PARENT_MINMAXH(i);\
        if (MIN_LEVEL_MINMAXH(i)) {\
            if (CMP(&h[i], &h[parent]) > 0) {\
                swap_minmaxh_##TYPENAME(&h[i], &h[parent]);\
                push_up_max_minmaxh_##TYPENAME(h, parent);\
            } else\
                push_up_min_minmaxh_##TYPENAME(h, i);\
        } else {\
            if (CMP(&h[i], &h[parent]) < 0) {\
                swap_minmaxh_##TYPENAME(&h[i], &h[parent]);\
                push_up_min_minmaxh_##TYPENAME(h, parent);\
            } else\
                push_up_max_minmaxh_##TYPENAME(h, i);\
        }\
    }\
}\
\
static void push_up_min_minmaxh_##TYPENAME(h, i)\
TYPENAME *h;\
{\
    int grandparent;\
\
    while (HAS_GRANDPARENT_MINMAXH(i) &&\
           CMP(&h[i], &h[(grandparent = GRANDPARENT_MINMAXH(i))]) < 0) {\
        swap_minmaxh_##TYPENAME(&h[i], &h[grandparent]);\
        i = grandparent;\
    }\
}\
\
static void push_up_max_minmaxh_##TYPENAME(h, i)\
TYPENAME *h;\
{\
    int grandparent;\
\
    while (HAS_GRANDPARENT_MINMAXH(i) &&\
           CMP(&h[i], &h[(grandparent = GRANDPARENT_MINMAXH(i))]) > 0) {\
        swap_minmaxh_##TYPENAME(&h[i], &h[grandparent]);\
        i = grandparent;\
    }\
}\
\
void remove_min_minmaxh_##TYPENAME(h, value)\
heap_minmaxh_##TYPENAME *h;\
TYPENAME *value;\
{\
    if (h->nmemb) {\
        memcpy(value, h->base, sizeof(TYPENAME));\
        memcpy(h->base, &h->base[--h->nmemb], sizeof(TYPENAME));\
        push_down_minmaxh_##TYPENAME(h->base, 0, h->nmemb);\
    }\
}\
\
void remove_max_minmaxh_##TYPENAME(h, value)\
heap_minmaxh_##TYPENAME *h;\
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
        push_down_minmaxh_##TYPENAME(h->base, i, h->nmemb);\
    }\
}\
\
static void push_down_minmaxh_##TYPENAME(h, i, nmemb)\
TYPENAME *h;\
{\
    if (MIN_LEVEL_MINMAXH(i))\
        push_down_min_minmaxh_##TYPENAME(h, i, nmemb);\
    else\
        push_down_max_minmaxh_##TYPENAME(h, i, nmemb);\
}\
\
static void swap_minmaxh_##TYPENAME(x, y)\
TYPENAME *x, *y;\
{\
    TYPENAME aux;\
\
    memcpy(&aux, x, sizeof(TYPENAME));\
    memcpy(x, y, sizeof(TYPENAME));\
    memcpy(y, &aux, sizeof(TYPENAME));\
}\
\
static void push_down_min_minmaxh_##TYPENAME(h, i, nmemb)\
TYPENAME *h;\
{\
    int m, g, parent;\
\
    while (HAS_CHILDREN_MINMAXH(i, nmemb)) {\
        g = 0;\
        m = smallest_minmaxh_##TYPENAME(h, i, &g, nmemb);\
        if (m >= 0) {\
            if (g && (CMP(&h[m], &h[i]) < 0)) {\
                swap_minmaxh_##TYPENAME(&h[m], &h[i]);\
                if ((parent = PARENT_MINMAXH(m)) >= 0)\
                    if (CMP(&h[m], &h[parent]) > 0)\
                        swap_minmaxh_##TYPENAME(&h[m], &h[parent]);\
                i = m;\
                continue;\
            } else if (CMP(&h[m], &h[i]) < 0)\
                swap_minmaxh_##TYPENAME(&h[m], &h[i]);\
        }\
        break;\
    }\
}\
\
static void push_down_max_minmaxh_##TYPENAME(h, i, nmemb)\
TYPENAME *h;\
{\
    int m, g, parent;\
\
    while (HAS_CHILDREN_MINMAXH(i, nmemb)) {\
        g = 0;\
        m = largest_minmaxh_##TYPENAME(h, i, &g, nmemb);\
        if (m >= 0) {\
            if (g && (CMP(&h[m], &h[i]) > 0)) {\
                swap_minmaxh_##TYPENAME(&h[m], &h[i]);\
                if ((parent = PARENT_MINMAXH(m)) >= 0)\
                    if (CMP(&h[m], &h[parent]) < 0)\
                        swap_minmaxh_##TYPENAME(&h[m], &h[parent]);\
                i = m;\
                continue;\
            } else if (CMP(&h[m], &h[i]) > 0)\
                swap_minmaxh_##TYPENAME(&h[m], &h[i]);\
        }\
        break;\
    }\
}\
\
static int smallest_minmaxh_##TYPENAME(h, i, g, nmemb)\
TYPENAME *h;\
int *g;\
{\
    int left, right, m;\
\
    m = left = LEFT_CHILD_MINMAXH(i, nmemb);\
    if (m < 0)\
        return m;\
    right = RIGHT_CHILD_MINMAXH(i, nmemb);\
    if (m+1 < nmemb && CMP(&h[m+1], &h[m]) < 0)\
        m++;\
    child_minmaxh_##TYPENAME(h, left, &m, g, -1, nmemb);\
    child_minmaxh_##TYPENAME(h, right, &m, g, -1, nmemb);\
    return m;\
}\
\
static int largest_minmaxh_##TYPENAME(h, i, g, nmemb)\
TYPENAME *h;\
int *g;\
{\
    int left, right, m;\
\
    m = left = LEFT_CHILD_MINMAXH(i, nmemb);\
    if (m < 0)\
        return m;\
    right = RIGHT_CHILD_MINMAXH(i, nmemb);\
    if (m+1 < nmemb && CMP(&h[m+1], &h[m]) > 0)\
        m++;\
    child_minmaxh_##TYPENAME(h, left, &m, g, 1, nmemb);\
    child_minmaxh_##TYPENAME(h, right, &m, g, 1, nmemb);\
    return m;\
}\
\
static void child_minmaxh_##TYPENAME(h, c, m, g, cmp, nmemb)\
TYPENAME *h;\
int *m, *g;\
{\
    int n, k, left;\
\
    if ((left = LEFT_CHILD_MINMAXH(c, nmemb)) >= 0)\
        for (n=2, k=left; k < nmemb && n--; k++)\
            if (cmp * (CMP(&h[k], &h[*m])) > 0) {\
                *g = 1;\
                *m = k;\
            }\
}
#endif
