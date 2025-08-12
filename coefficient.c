#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "getop.c"

/*gcc coefficient.c getch.c atof.c round.c -lm*/

#define TRUE 1
#define FALSE 0
#define LEFT 0
#define RIGHT 1
#define DELIMITER ';'
#define STEP 2

struct vector {
        double (*v)[2];
        int size, end;
} v;

int columns = FALSE;

int main(argc, argv)
int argc;
char *argv[];
{
	void read_nums();
	FILE *fp;
	
	v.v = (double (*)[2]) malloc(sizeof(double)*2);
	v.size = 1;
	
	if (argc >= 2)
		if (argv[1][0] == '-') {
			argc--;
			if ((*++argv)[1] == 'c')
				columns = TRUE;
			else
				fprintf(stderr, "coefficient: parâmetro inválido %c\n", argv[1][1]);
		}
	if (argc <= 1)
		read_nums(stdin);
	else
		while (--argc > 0)
			if ((fp = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "coefficient: arquivo não encontrado: %s\n", *argv);
				exit(1);
			} else {
				read_nums(fp);
				fclose(fp);
			}
	exit(0);
}

static int one;

void read_nums(fp)
FILE *fp;
{
        extern double atof();
        void insert_tuple(), estatistics();
        char s[MAXOP];
        int type, side = LEFT;

	v.end = 0;
	one = FALSE;
	while ((type = getop(s, MAXOP, fp)) != EOF)
		if (type == NUMBER) {
			insert_tuple(atof(s), &v, side);
			if (columns == FALSE)
				side = !side;
		} else if (type == DELIMITER && columns)
			side = RIGHT;
        estatistics(v.v, v.end);
        putchar('\n');
}

void insert_tuple(x, v, i)
double x;
struct vector *v;
int i;
{

	if (i == RIGHT && columns && one == FALSE) {
		v->end = 0;
		one = TRUE;
	}
	if (v->end == v->size)
                v->v = (double (*)[2]) realloc(v->v, sizeof(double)*2 * (v->size *= STEP));
        v->v[v->end][i] = x;
	if ((columns==FALSE && i==RIGHT) || columns)
		v->end++;
}

void estatistics(v, n)
double (*v)[2];
int n;
{
	extern double round_d();
	double (*ptr)[2];
	double sumx, sumy, sumx2, sumy2, sumxy;
	double r, a, b, nume, deno;
	char *correlation;

	sumx = sumy = sumx2 = sumy2 = sumxy = .0;
	for (ptr=v; ptr<v+n; ptr++) {
		sumx += (*ptr)[LEFT];
		sumy += (*ptr)[RIGHT];
		sumx2 += pow((*ptr)[LEFT], 2);
		sumy2 += pow((*ptr)[RIGHT], 2);
		sumxy += (*ptr)[LEFT] * (*ptr)[RIGHT];
	}
	nume = n*sumxy-sumx*sumy;
	deno = n*sumx2-pow(sumx,2);
	printf("%.3lf %.3lf %.3lf %.3lf %.3lf %.3lf %.3lf\n", nume, deno, sumx, sumx2, sumy, sumy2, sumxy);

	b = nume/deno;
	a = sumy/n - b*(sumx/n);
	r = nume/sqrt(deno*(n*sumy2-pow(sumy,2)));
	printf("r: %.4lf\na: %.2lf\tb: %.2lf\n", trunc(r*10000)/10000, a, b);

	r = fabs(r);
	if (r == .0)
		correlation = "nula";
	else if (r < .3)
		correlation = "fraca";
	else if (r < .6)
		correlation = "regular";
	else if (r < .9)
		correlation = "forte";
	else if (r < 1.0)
		correlation = "muito forte";
	else if (r == 1.0)
		correlation = "perfeita (plena)";
	printf("A correlação é %s.", correlation);
}
