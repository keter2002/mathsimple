#include <stdio.h>
#include <stdlib.h>
#include "mathfn.h"
#include "torfnum.h"

main(argc, argv)
char *argv[];
{
	double num, x, rt, numerator, m, aux, exp;
	int i, j, k, fat, it;

	if (argc < 3) {
		fputs("Usage: number root iterations\n", stderr);
		return 2;
	}
	num = atof_torfnum(argv[1]);
	exp = atof_torfnum(argv[2]);
	it = argc > 3? atoi_torfnum(argv[3]) : 0;
	if (num > 1.0) {
		i = 0;
		do {
			i++;
			aux = powd_mathfn((double)i, exp);
		} while (aux < num);
		if (!i) {
			fputs("Very small number.\n", stderr);
			return 1;
		}
		i -= 1;
		x = (num-powd_mathfn((double)i, exp))/powd_mathfn((double)i, exp);
	} else
		x = num;	
	rt = 1;
	for (j=1; j < it; j++) {
		numerator = m = exp;
		fat = 1;
		for (k=1; k < j; k++) {
			numerator *= m - k;	
			fat *= k+1;
		}
		m = numerator/fat;
		rt += m*powd_mathfn(x, j);
		printf("%f %f %f\n", rt, m, powd_mathfn(x,j));
	}
	printf("%.5lf\n", num > 1.0? rt*i : rt);
}
