#include <stdio.h>
#include <math.h>
#include "mathfn.h"

#define RANGE 13

void print_elifs(char *sig)
{
	int i,j,k;
	double ilf, sqrt_i;

	for (i=1; i <= RANGE; i++) {
		ilf = i;
		sqrt_i = sqrt(ilf);
		if (ceil(sqrt_i) != sqrt_i)
			printf(" else if (%sSQRT_%02d_LA - EPS_LA < x && %sSQRT_%02d_LA + EPS_LA > x) { \\\n"
				"    printf(\"%s%d^(1/2) \");\\\n"
				"}", sig, i, sig, i, sig, i);
		for (j=2; j <= RANGE; j++) {
			if (greatest_common_divisor_mathfn(i, j) == 1)
				printf(" %sif (%sN%02d_OVER_%02d_LA - EPS_LA < x && %sN%02d_OVER_%02d_LA + EPS_LA > x) { \\\n"
					"    printf(\"%s%d/%d \");\\\n"
					"}", i==1 && j==2 && !*sig? "" : "else ", sig, i, j, sig, i, j, sig, i, j);
			if (ceil(sqrt_i) != sqrt_i) {
				/* SQRT_TIMES */
				printf(" else if (%sSQRT_%02d_TIMES_%02d_LA - EPS_LA < x && %sSQRT_%02d_TIMES_%02d_LA + EPS_LA > x) { \\\n"
					"    printf(\"%s%d.%d^(1/2) \");\\\n"
					"}", sig, i, j, sig, i, j, sig, j, i);
				/* SQRT_OVER */
				printf(" else if (%sSQRT_%02d_OVER_%02d_LA - EPS_LA < x && %sSQRT_%02d_OVER_%02d_LA + EPS_LA > x) { \\\n"
					"    printf(\"%s%d^(1/2)/%d \");\\\n"
					"}", sig, i, j, sig, i, j, sig, i, j);
				for (k=2; k <= RANGE; k++)
					if (greatest_common_divisor_mathfn(j, k) == 1)
						printf(" else if (%sSQRT_%02d_TIMES_%02d_OVER_%02d_LA - EPS_LA < x && %sSQRT_%02d_TIMES_%02d_OVER_%02d_LA + EPS_LA > x) { \\\n"
							"    printf(\"%s%d.%d^(1/2)/%d \");\\\n"
							"}", sig, i, j, k, sig, i, j, k, sig, j, i, k);
			}
		}
	}
}

main()
{
	int i, j, k;
	double ilf, jlf, klf;
	double sqrt_i;
	
	puts("#ifndef H_PRINT_KNOW_CONSTANT_LA");
	puts("#define H_PRINT_KNOW_CONSTANT_LA\n");
	/* Frações.	 */
	for (i=1; i <= RANGE; i++) {
		ilf = i;
		sqrt_i = sqrt(ilf);
		if (ceil(sqrt_i) != sqrt_i)
			printf("#define SQRT_%02d_LA %.14lf\n", i, sqrt(ilf));
		for (j=2; j <= RANGE; j++) {
			jlf = j;
			if (greatest_common_divisor_mathfn(i, j) == 1)
				printf("#define N%02d_OVER_%02d_LA %.14lf\n", i, j, i/jlf);
			if (ceil(sqrt_i) != sqrt_i) {
				printf("#define SQRT_%02d_TIMES_%02d_LA %.14lf\n", i, j, sqrt(ilf) * j);
				printf("#define SQRT_%02d_OVER_%02d_LA %.14lf\n", i, j, sqrt(ilf) / jlf);
				for (k=2; k <= RANGE; k++)
					if (greatest_common_divisor_mathfn(j, k) == 1) {
						klf = k;
						printf("#define SQRT_%02d_TIMES_%02d_OVER_%02d_LA %.14lf\n", i, j, k, sqrt(ilf)*j/klf);
					}
			}
		}
	}
	puts("\n#define PRINT_KNOW_CONSTANT_LA(x)\\");
	print_elifs("");
	print_elifs("-");
	puts("\n\n#endif");
}
