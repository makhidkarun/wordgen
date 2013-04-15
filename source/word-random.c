#ifdef BSD
#define MAX_RAND	2147483648
#else
#define MAX_RAND	32767
#endif

/* random(n) - a random integer generator with 20 bit resolution */

int random_first = 1;

random(n)	/* generate a number from 1 to n, n < 1.0EE6 */
int n;
{
	float fval;
	int val;

	if (random_first) {
		srand((unsigned int) time((long *)0));
		random_first=0;
	}
	fval = ((float) rand()) / MAX_RAND;
	val = fval * ((float) n);
        val = abs(val);
	return(++val);
}
