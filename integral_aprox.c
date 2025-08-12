#include "expression.c"
#include <stdio.h>
#include <stdlib.h>

dynamic_array fullexp;

main(argc,argv)
char *argv[];
{
	void infix_posfix();
	double right_integral(), left_integral(), middle_integral(), trapezoid(), simpson();
	double a,b,n;

	if (argc < 5) {
		fputs("Usage: expression a b n\n", stderr);
		return 2;
	}
	infix_posfix(&fullexp, argv[1]);
	show_expr(&fullexp);
	a = atof(argv[2]); b = atof(argv[3]); n = atof(argv[4]);
	printf("RI: %lf\n", right_integral(a, b, n));
	printf("LI: %lf\n", left_integral(a, b, n));
	printf("MI: %lf\n", middle_integral(a, b, n));
	printf("Trapezoidal Rule: %lf\n", trapezoid(a,b,n));
	if (!((int)n%2))
		printf("Simpson Rule: %lf\n", simpson(a,b,n));
}

double right_integral(a, b, n)
double a,b,n;
{
	double inc, res, x;
	int i;

	inc = (b-a)/n;
	printf("%lf %lf %lf %lf\n", a, b, n, inc);
	printf("RI - LI = %lf\n", inc*(evaluate(&fullexp, b)-evaluate(&fullexp, a)));
	res = 0;
	for (x=a+inc,i=0; i < n; x+=inc, i++)
		res += evaluate(&fullexp, x);
	return res * inc;
}

double left_integral(a,b,n)
double a,b,n;
{
	double inc, res, x;
	int i;

	inc = (b-a)/n;
	res = 0;
	for (x=a,i=0; i < n; x+=inc, i++)
		res += evaluate(&fullexp, x);
	return res * inc;
}

double middle_integral(a,b,n)
double a,b,n;
{
	double inc, res, x;
	int i;

	inc = (b-a)/n;
	res = 0;
	for (x=a+inc/2.0f,i=0; i < n; x+=inc,i++)
		res += evaluate(&fullexp, x);
	return res * inc;
}

double trapezoid(a,b,n)
double a,b,n;
{
	double inc, res, x;
	int i;

	inc = (b-a)/n;
	x=a;
	res = evaluate(&fullexp, x)/2.0;
	for (x+=inc, i=1; i < n; x+=inc,i++)
		res += evaluate(&fullexp, x);
	res += evaluate(&fullexp, x)/2.0;
	return res * inc;
}

double simpson(a,b,n)
double a,b,n;
{
	double inc, res, x;
	int i;

	inc = (b-a)/n;
	x=a;
	res = evaluate(&fullexp, x);
	for (x+=inc, i=1; i < n; x+=inc,i++)
		res += i%2? 4*evaluate(&fullexp, x) : 2*evaluate(&fullexp, x);
	res += evaluate(&fullexp, x);
	return res * inc / 3.0;
}
