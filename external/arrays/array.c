/*
    Generic dynamic array implementation in C.
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

#ifndef H_ARRAY
#define H_ARRAY

#include <stdio.h>
#include <stdlib.h>


typedef struct {
    void *base;
    size_t size, nmemb, capacity;
} dynamic_array;

#define allocate_ptr_array(ARR, SIZE, CAP) \
    do {\
        (ARR)->size = SIZE; \
        (ARR)->capacity = CAP; \
        (ARR)->base = malloc((ARR)->size * (ARR)->capacity); \
        if (!(ARR)->base) { \
            fputs("[ALLOCATE_PTR_ARRAY] Not enough memory.\n", stderr); \
            exit(1); \
        } \
        (ARR)->nmemb = 0;\
    } while (0)
#define allocate_array(ARR, SIZE, CAP) allocate_ptr_array(&(ARR), SIZE, CAP)

#define LAST_SPACE_PTR_ARRAY(ARR) ((ARR)->base + (ARR)->size * (ARR)->nmemb)
#define LAST_SPACE_ARRAY(ARR) LAST_SPACE_PTR_ARRAY(&(ARR))

#define AT_PTR_ARRAY(ARR, I) ((ARR)->base + (I) * (ARR)->size)
#define AT_ARRAY(ARR, I) AT_PTR_ARRAY(&(ARR), I)

#define expand_ptr_array(ARR, FAC, INC) \
    do { \
        void *tmp; \
        \
        if ((ARR)->nmemb == (ARR)->capacity) { \
            (ARR)->capacity = (ARR)->capacity * (FAC) + (INC); \
            if ((tmp = realloc((ARR)->base, (ARR)->size * (ARR)->capacity))) \
                (ARR)->base = tmp; \
            else { \
                fputs("[EXPAND_PTR_ARRAY] Realloc fail.\n", stderr); \
                exit(1); \
            } \
        } \
    } while (0)
#define expand_array(ARR, FAC, INC) expand_ptr_array(&(ARR), FAC, INC)
#endif
