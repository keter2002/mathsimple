/*
    generate_header - v1.1.0
    Program to generate LA_PRINT_KNOW_CONSTANT macro. This macro gets a
    floating point number and prints equivalent mathematical symbol.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    generate_header is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#include <stdio.h>
#include <math.h>
#include "mathfn.h"

#define DECIMAL_PLACES "14"
#define RANGE 13

void print_elifs(char *sig)
{
    int i,j,k;
    double ilf, sqrt_lf;

    for (i=1; i <= RANGE; i++) {
        ilf = i;
        sqrt_lf = sqrt(ilf);
        if (ceil(sqrt_lf) != sqrt_lf)
            printf(" else if (%sLA_SQRT_%02d - LA_EPS < x && %sLA_SQRT_%02d + LA_EPS > x) { \\\n"
                "    printf(\"%s%d^(1/2) \");\\\n"
                "}", sig, i, sig, i, sig, i);
        for (j=2; j <= RANGE; j++) {
            if (mathfn_greatest_common_divisor(i, j) == 1)
                printf(" %sif (%sLA_%02d_OVER_%02d - LA_EPS < x && %sLA_%02d_OVER_%02d + LA_EPS > x) { \\\n"
                    "    printf(\"%s%d/%d \");\\\n"
                    "}", i==1 && j==2 && !*sig? "" : "else ", sig, i, j, sig, i, j, sig, i, j);
            if (ceil(sqrt_lf) != sqrt_lf) {
                /* SQRT_TIMES */
                printf(" else if (%sLA_SQRT_%02d_TIMES_%02d - LA_EPS < x && %sLA_SQRT_%02d_TIMES_%02d + LA_EPS > x) { \\\n"
                    "    printf(\"%s%d.%d^(1/2) \");\\\n"
                    "}", sig, i, j, sig, i, j, sig, j, i);
                /* SQRT_OVER */
                printf(" else if (%sLA_SQRT_%02d_OVER_%02d - LA_EPS < x && %sLA_SQRT_%02d_OVER_%02d + LA_EPS > x) { \\\n"
                    "    printf(\"%s%d^(1/2)/%d \");\\\n"
                    "}", sig, i, j, sig, i, j, sig, i, j);
                for (k=2; k <= RANGE; k++)
                    if (mathfn_greatest_common_divisor(j, k) == 1)
                        printf(" else if (%sLA_SQRT_%02d_TIMES_%02d_OVER_%02d - LA_EPS < x && %sLA_SQRT_%02d_TIMES_%02d_OVER_%02d + LA_EPS > x) { \\\n"
                            "    printf(\"%s%d.%d^(1/2)/%d \");\\\n"
                            "}", sig, i, j, k, sig, i, j, k, sig, j, i, k);
            }
        }
    }
}

void print_defines()
{
    int i, j, k;
    double ilf, jlf, klf;
    double sqrt_lf;

    /* Fractions. */
    for (i=1; i <= RANGE; i++) {
        ilf = i;
        sqrt_lf = sqrt(ilf);
        if (ceil(sqrt_lf) != sqrt_lf)
            printf("#define LA_SQRT_%02d %." DECIMAL_PLACES "lf\n", i, sqrt_lf);
        for (j=2; j <= RANGE; j++) {
            jlf = j;
            if (mathfn_greatest_common_divisor(i, j) == 1)
                printf("#define LA_%02d_OVER_%02d %." DECIMAL_PLACES "lf\n", i, j, i/jlf);
            if (ceil(sqrt_lf) != sqrt_lf) {
                printf("#define LA_SQRT_%02d_TIMES_%02d %." DECIMAL_PLACES "lf\n", i, j, sqrt_lf * j);
                printf("#define LA_SQRT_%02d_OVER_%02d %." DECIMAL_PLACES "lf\n", i, j, sqrt_lf / jlf);
                for (k=2; k <= RANGE; k++)
                    if (mathfn_greatest_common_divisor(j, k) == 1) {
                        klf = k;
                        printf("#define LA_SQRT_%02d_TIMES_%02d_OVER_%02d %." DECIMAL_PLACES "lf\n", i, j, k, sqrt_lf*j/klf);
                    }
            }
        }
    }
}

typedef struct {
    double val; 
    char *str, *symb;
} math_constant;

math_constant math_constants[] = {{M_E, "M_E", "e"},
                                  {M_LOG2E, "M_LOG2E", "log_2(e)"},
                                  {M_LOG10E, "M_LOG10E", "log_10(e)"},
                                  {M_LN2, "M_LN2", "ln(2)"},
                                  {M_LN10, "M_LN10", "ln(10)"},
                                  {M_PI, "M_PI", "pi"}};

void print_math_constants_elifs(char *sig) {
    int i, j, k;
    char *str, *symb;

    for (i=0; i < sizeof(math_constants)/sizeof(math_constant); i++) {
        str = math_constants[i].str;
        symb = math_constants[i].symb;

        /* SQRT */
        printf(" else if (%sLA_SQRT_%s - LA_EPS < x && %sLA_SQRT_%s + LA_EPS > x) { \\\n"
            "    printf(\"%s(%s)^(1/2) \");\\\n"
            "}", sig, str, sig, str, sig, symb);
        /* SQUARED */
        printf(" else if (%sLA_SQUARED_%s - LA_EPS < x && %sLA_SQUARED_%s + LA_EPS > x) { \\\n"
            "    printf(\"%s(%s)^2 \");\\\n"
            "}", sig, str, sig, str, sig, symb);
        for (j=2; j <= RANGE; j++) {
            /* OVER */
            printf(" else if (%sLA_%s_OVER_%02d - LA_EPS < x && %sLA_%s_OVER_%02d + LA_EPS > x) { \\\n"
                "    printf(\"%s%s/%d \");\\\n"
                "}", sig, str, j, sig, str, j, sig, symb, j);
            /* SQRT_TIMES */
            printf(" else if (%sLA_SQRT_%s_TIMES_%02d - LA_EPS < x && %sLA_SQRT_%s_TIMES_%02d + LA_EPS > x) { \\\n"
                "    printf(\"%s%d.(%s)^(1/2) \");\\\n"
                "}", sig, str, j, sig, str, j, sig, j, symb);
            /* SQRT_OVER */
            printf(" else if (%sLA_SQRT_%s_OVER_%02d - LA_EPS < x && %sLA_SQRT_%s_OVER_%02d + LA_EPS > x) { \\\n"
                "    printf(\"%s(%s)^(1/2)/%d \");\\\n"
                "}", sig, str, j, sig, str, j, sig, symb, j);
            /* SQUARED_TIMES */
            printf(" else if (%sLA_SQUARED_%s_TIMES_%02d - LA_EPS < x && %sLA_SQUARED_%s_TIMES_%02d + LA_EPS > x) { \\\n"
                "    printf(\"%s%d.(%s)^2 \");\\\n"
                "}", sig, str, j, sig, str, j, sig, j, symb);
            /* SQUARED_OVER */
            printf(" else if (%sLA_SQUARED_%s_OVER_%02d - LA_EPS < x && %sLA_SQUARED_%s_OVER_%02d + LA_EPS > x) { \\\n"
                "    printf(\"%s(%s)^2/%d \");\\\n"
                "}", sig, str, j, sig, str, j, sig, symb, j);
        }
        for (j=2; j <= RANGE; j++)
            for (k=2; k <= RANGE; k++)
                if (mathfn_greatest_common_divisor(j, k) == 1) {
                    /* TIMES_OVER */
                    printf(" else if (%sLA_%s_TIMES_%02d_OVER_%02d - LA_EPS < x && %sLA_%s_TIMES_%02d_OVER_%02d + LA_EPS > x) { \\\n"
                        "    printf(\"%s%d.%s/%d \");\\\n"
                        "}", sig, str, j, k, sig, str, j, k, sig, j, symb, k);
                    /* SQRT_TIMES_OVER */
                    printf(" else if (%sLA_SQRT_%s_TIMES_%02d_OVER_%02d - LA_EPS < x && %sLA_SQRT_%s_TIMES_%02d_OVER_%02d + LA_EPS > x) { \\\n"
                        "    printf(\"%s%d.(%s)^(1/2)/%d \");\\\n"
                        "}", sig, str, j, k, sig, str, j, k, sig, j, symb, k);
                    /* SQUARED_TIMES_OVER */
                    printf(" else if (%sLA_SQUARED_%s_TIMES_%02d_OVER_%02d - LA_EPS < x && %sLA_SQUARED_%s_TIMES_%02d_OVER_%02d + LA_EPS > x) { \\\n"
                        "    printf(\"%s%d.(%s)^2/%d \");\\\n"
                        "}", sig, str, j, k, sig, str, j, k, sig, j, symb, k);
                }
    }
}

void print_math_constants_defines()
{
    int i, j, k;
    double jlf, klf, sqrt_lf, squared, val;
    char *str;

    for (i=0; i < sizeof(math_constants)/sizeof(math_constant); i++) {
        val = math_constants[i].val;
        str = math_constants[i].str;
        squared = val*val;
        sqrt_lf = sqrt(val);
        printf("#define LA_SQRT_%s %." DECIMAL_PLACES "lf\n", str, sqrt_lf);
        printf("#define LA_SQUARED_%s %." DECIMAL_PLACES "lf\n", str, squared);
        for (j=2; j <= RANGE; j++) {
            jlf = j;
            printf("#define LA_%s_OVER_%02d %." DECIMAL_PLACES "lf\n", str, j, val / jlf);
            printf("#define LA_SQRT_%s_TIMES_%02d %." DECIMAL_PLACES "lf\n", str, j, sqrt_lf * j);
            printf("#define LA_SQRT_%s_OVER_%02d %." DECIMAL_PLACES "lf\n", str, j, sqrt_lf / jlf);
            printf("#define LA_SQUARED_%s_TIMES_%02d %." DECIMAL_PLACES "lf\n", str, j, squared * j);
            printf("#define LA_SQUARED_%s_OVER_%02d %." DECIMAL_PLACES "lf\n", str, j, squared / jlf);
        }
        for (j=2; j <= RANGE; j++)
            for (k=2; k <= RANGE; k++)
                if (mathfn_greatest_common_divisor(j, k) == 1) {
                    klf = k;
                    printf("#define LA_%s_TIMES_%02d_OVER_%02d %." DECIMAL_PLACES "lf\n", str, j, k, val*j/klf);
                    printf("#define LA_SQRT_%s_TIMES_%02d_OVER_%02d %." DECIMAL_PLACES "lf\n", str, j, k, sqrt_lf*j/klf);
                    printf("#define LA_SQUARED_%s_TIMES_%02d_OVER_%02d %." DECIMAL_PLACES "lf\n", str, j, k, squared*j/klf);
                }
    }
}

main()
{
    puts("#ifndef LA_PRINT_KNOW_CONSTANT_H\n"
         "#define LA_PRINT_KNOW_CONSTANT_H\n");
    
    print_defines();
    print_math_constants_defines();

    puts("\n#define LA_PRINT_KNOW_CONSTANT(x)\\");
    print_elifs("");
    print_elifs("-");
    print_math_constants_elifs("");
    print_math_constants_elifs("-");
    puts("\n\n#endif");
}
