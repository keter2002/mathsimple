double round_d(x)
double x;
{
	return (x > 0? (int)(100*x+0.5)/100.0 : (int)(100*x-0.5)/100.0);
}
