#include <stdio.h>

#include "mathfn.h"
#include "know_constant.h"

know_constant_math_constant num;
arraytyped_array_know_constant_math_constant know;

main()
{
    void append_numbers(), append_constants(), serializer_to();

    arraytyped_allocate(know_constant_math_constant, know, 2500);
    append_numbers();
    append_constants();
    arraytyped_qsort(know_constant_math_constant, know, know_constant_compar_math_constant);
    serializer_to(KNOW_CONSTANT_FILENAME);
    free(know.base);
}                                                                                                         
                                                                                                          
void append_numbers()
{
    int i,j,k;
    double ilf, jlf, klf, sqrt_lf;

    for (i=1; i <= KNOW_CONSTANT_RANGE; i++) {
        ilf = i;
        sqrt_lf = sqrt(ilf);
        if (ceil(sqrt_lf) != sqrt_lf) {
            num.val = sqrt_lf;
            snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d^(1/2)", i);
            arraytyped_append_to_end(know_constant_math_constant, know, &num);
        }
        for (j=2; j <= KNOW_CONSTANT_RANGE; j++) {
            jlf = j;
            if (mathfn_greatest_common_divisor(i, j) == 1) {
                num.val = i/jlf;
                snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d/%d", i, j);
                arraytyped_append_to_end(know_constant_math_constant, know, &num);
            }
            if (ceil(sqrt_lf) != sqrt_lf) {
                /* SQRT_TIMES */
                num.val = sqrt_lf * j;
                snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d.%d^(1/2)", j, i);
                arraytyped_append_to_end(know_constant_math_constant, know, &num);
                /* SQRT_OVER */
                num.val = sqrt_lf / jlf;
                snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d^(1/2)/%d", i, j);
                arraytyped_append_to_end(know_constant_math_constant, know, &num);
                for (k=2; k <= KNOW_CONSTANT_RANGE; k++)
                    if (mathfn_greatest_common_divisor(j, k) == 1) {
                        klf = k;
                        num.val = sqrt_lf*j/klf;
                        snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d.%d^(1/2)/%d", j, i, k);
                        arraytyped_append_to_end(know_constant_math_constant, know, &num);
                    }
            }
        }
    }
}

void append_constants()
{
    int i, j, k;
    double jlf, klf, sqrt_lf, squared, val;
    char *symb;

    for (i=0; i < sizeof(know_constant_constants) /
                  sizeof(know_constant_math_constant) - 1; i++) {
        val = know_constant_constants[i].c.val;
        symb = know_constant_constants[i].c.symb;

        /* SQRT */
        sqrt_lf = num.val = sqrt(val);
        snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "(%s)^(1/2)", symb);
        arraytyped_append_to_end(know_constant_math_constant, know, &num);
        /* SQUARED */
        squared = num.val = val*val;
        snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "(%s)^2", symb);
        arraytyped_append_to_end(know_constant_math_constant, know, &num);
        for (j=2; j <= KNOW_CONSTANT_RANGE; j++) {
            jlf = j;
            /* OVER */
            num.val = val / jlf;
            snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%s/%d", symb, j);
            arraytyped_append_to_end(know_constant_math_constant, know, &num);
            /* SQRT_TIMES */
            num.val = sqrt_lf * j;
            snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d.(%s)^(1/2)", j, symb);
            arraytyped_append_to_end(know_constant_math_constant, know, &num);
            /* SQRT_OVER */
            num.val = sqrt_lf / jlf;
            snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "(%s)^(1/2)/%d", symb, j);
            arraytyped_append_to_end(know_constant_math_constant, know, &num);
            /* SQUARED_TIMES */
            num.val = squared * j;
            snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d.(%s)^2", j, symb);
            arraytyped_append_to_end(know_constant_math_constant, know, &num);
            /* SQUARED_OVER */
            num.val = squared / jlf;
            snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "(%s)^2/%d", symb, j);
            arraytyped_append_to_end(know_constant_math_constant, know, &num);
        }
        for (j=2; j <= KNOW_CONSTANT_RANGE; j++)
            for (k=2; k <= KNOW_CONSTANT_RANGE; k++)
                if (mathfn_greatest_common_divisor(j, k) == 1) {
                    klf = k;
                    /* TIMES_OVER */
                    num.val = val*j/klf;
                    snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d.%s/%d", j, symb, k);
                    arraytyped_append_to_end(know_constant_math_constant, know, &num);
                    /* SQRT_TIMES_OVER */
                    num.val = sqrt_lf*j/klf;
                    snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d.(%s)^(1/2)/%d", j, symb, k);
                    arraytyped_append_to_end(know_constant_math_constant, know, &num);
                    /* SQUARED_TIMES_OVER */
                    num.val = squared*j/klf;
                    snprintf(num.symb, KNOW_CONSTANT_MAX_SYMB_LEN, "%d.(%s)^2/%d", j, symb, k);
                    arraytyped_append_to_end(know_constant_math_constant, know, &num);
                }
    }
}

void serializer_to(char filename[])
{
    FILE *fp;

    if (!(fp = fopen(filename, "wb"))) {
        fprintf(stderr, "[%s:%s] could not open for writing (%s).\n", __FILE__,
                __func__, filename);
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%lx\n", know.nmemb);
    fwrite(know.base, sizeof(know_constant_math_constant), know.nmemb, fp);
    fclose(fp);
}
