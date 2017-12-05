#include <cstdio>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <ctime>
#include <omp.h>
#include <tuple>

using std::__gcd;
typedef std::tuple<unsigned long long, unsigned long long, unsigned long long> int_tuple;

void input(int_tuple *integers, int len, char filename[])
{
	FILE *fin = fopen(filename, "r");
	if (fin == nullptr) {
		fprintf(stderr, "Error: cannot open file %s\n.", filename);
		exit(1);
	}

	for (int i = 0; i < len; ++i) {
		unsigned long long big, p1, p2;
		fscanf(fin, "%llu %llu %llu", &big, &p1, &p2);
		integers[i] = std::make_tuple(big, p1, p2);
	}
	

	fclose(fin);
}

/* result  = b^e mod m */
unsigned long long modular_pow(
		unsigned long long base, 
		int exponent,
		unsigned long long modulus)
{
	/* initialize result */
	unsigned long long result = 1;

	while (exponent > 0)
	{
		/* if y is odd, multiply base with result */
		if (exponent & 1)
			result = (result * base) % modulus;

		/* exponent = exponent/2 */
		exponent = exponent >> 1;

		/* base = base * base */
		base = (base * base) % modulus;
	}
	return result;
}

unsigned long long pr(unsigned long long n)
{
	srand(time(NULL));

	/* no prime divisor for 1 */
	if (n == 1) {
		return n;
	}
	/* even number means one of the divisors is 2 */
	if ((n & 1) == 0) {
		return 2;
	}
	/* we will pick from the range [2, N) */
	unsigned long long x = (rand() % (n - 2)) + 2;
	unsigned long long y = x;
	/* the constant in f(x). */
	unsigned long long c = (rand() % (n - 1)) + 1;
	/* Initialize candidate divisor (or result) */
	unsigned long long d = 1;  
	/* f() = x^2 + c mod n */
	auto f = [=](unsigned long long x) -> unsigned long long {
	   	return (modular_pow(x, 2, n) + c) % n;
	};
	/* find the divisor */
	while (d == 1)
	{
		/* Tortoise Move: x(i+1) = f(x(i)) */
		x = f(x);
		/* Hare Move: y(i+1) = f(f(y(i))) */
		y = f(y);
		y = f(y);
		/* check gcd of |x-y| and n */
		d = __gcd((unsigned long long) llabs(x-y), n);
	}
	if (d == n) {
		return 0;
	} else {
		return d;
	}
}

int main(int argc, char *argv[])
{
	const int len = 10000;
	//int_tuple integers[len];
	int_tuple *integers = new int_tuple[len];
	unsigned long long n;
	//if (argc > 1) {
	//	n = strtoull(argv[1], nullptr, 10);
	//} else {
	//	n = 10967535067;
	//}

	/* Input data
	   */
	input(integers, len, "data/integers.txt");
	double start_time = omp_get_wtime();
	for (int i = 0; i < len; ++i) {
		unsigned long long x = std::get<0>(integers[i]);
		unsigned long long factor = 0;
		while (0 == factor) {
			factor = pr(x);
		}
		//printf("factor: %llu\n", factor);
		unsigned long long p1 = std::get<1>(integers[i]);
		unsigned long long p2 = std::get<2>(integers[i]);
		if (factor != p1 && factor != p2) {
			printf("Wrong: x: %llu, factor: %llu, p1: %llu, p2: %llu\n", x, factor, p1, p2);
		}
	}
	double run_time = omp_get_wtime() - start_time;
	printf("time: %f\n", run_time);

	delete [] integers;

	return 0;
}
