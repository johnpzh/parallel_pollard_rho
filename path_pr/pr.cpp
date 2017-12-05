#include <cstdio>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <ctime>
#include <omp.h>
#include <tuple>

int NUM_THREADS;

using std::__gcd;
typedef unsigned long long Ullint;
typedef std::tuple<Ullint, Ullint, Ullint> int_tuple;

void input(int_tuple *integers, int len, char filename[])
{
	FILE *fin = fopen(filename, "r");
	if (fin == nullptr) {
		fprintf(stderr, "Error: cannot open file %s\n.", filename);
		exit(1);
	}

	for (int i = 0; i < len; ++i) {
		Ullint big, p1, p2;
		fscanf(fin, "%llu %llu %llu", &big, &p1, &p2);
		integers[i] = std::make_tuple(big, p1, p2);
	}
	

	fclose(fin);
}

/* result  = b^e mod m */
Ullint modular_pow(
		Ullint base, 
		int exponent,
		Ullint modulus)
{
	/* initialize result */
	Ullint result = 1;

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

Ullint pr(Ullint n)
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

	Ullint *results = (Ullint *) malloc(sizeof(Ullint) * NUM_THREADS);
	int found = 0;
	int founder = 0;

	omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
{
	int tid = omp_get_thread_num();
	/* we will pick from the range [2, N) */
	Ullint x = (rand() % (n - 2)) + 2;
	Ullint y = x;
	/* the constant in f(x). */
	Ullint c = (rand() % (n - 1)) + 1;
	/* f() = x^2 + c mod n */
	auto f = [=](Ullint x) -> Ullint {
	   	return (modular_pow(x, 2, n) + c) % n;
	};
	/* Initialize candidate divisor (or result) */
	Ullint d = 1;  
	/* find the divisor */
	while (d == 1 && !found)
	{
		/* Tortoise Move: x(i+1) = f(x(i)) */
		x = f(x);
		/* Hare Move: y(i+1) = f(f(y(i))) */
		y = f(y);
		y = f(y);
		/* check gcd of |x-y| and n */
		d = __gcd((Ullint) llabs(x-y), n);
	}
	if (d != n && !found) {
		found = 1;
		results[tid] = d;
		founder = tid;
	} 
}
	Ullint result = results[founder];
	free(results);
	if (found) {
		return result;
	} else {
		return 0;
	}

	///* we will pick from the range [2, N) */
	//Ullint x = (rand() % (n - 2)) + 2;
	//Ullint y = x;
	///* the constant in f(x). */
	//Ullint c = (rand() % (n - 1)) + 1;
	///* Initialize candidate divisor (or result) */
	//Ullint d = 1;  
	///* find the divisor */
	//while (d == 1)
	//{
	//	/* Tortoise Move: x(i+1) = f(x(i)) */
	//	x = f(x);
	//	/* Hare Move: y(i+1) = f(f(y(i))) */
	//	y = f(y);
	//	y = f(y);
	//	/* check gcd of |x-y| and n */
	//	d = __gcd((Ullint) llabs(x-y), n);
	//}

	//if (d == n) {
	//	return 0;
	//} else {
	//	return d;
	//}
}

int main(int argc, char *argv[])
{
	const int len = 10000;
	//const int len = 1000000;
	//int_tuple integers[len];
	int_tuple *integers = new int_tuple[len];
	Ullint n;
	//if (argc > 1) {
	//	n = strtoull(argv[1], nullptr, 10);
	//} else {
	//	n = 10967535067;
	//}

	/* Input data */
	input(integers, len, "/home/zpeng/cs680/data/integers.txt");
	//input(integers, len, "/home/zpeng/cs680/data/integers.txt.1m");

	/* Compute */
	for (int i = 1; i < 65; i *= 2) {
		NUM_THREADS = i;
		double start_time = omp_get_wtime();
		for (int i = 0; i < len; ++i) {
			Ullint x = std::get<0>(integers[i]);
			Ullint factor = 0;
			while (0 == factor) {
				factor = pr(x);
			}
			/* Testing */
			//printf("factor: %llu\n", factor);
			Ullint p1 = std::get<1>(integers[i]);
			Ullint p2 = std::get<2>(integers[i]);
			if (factor != p1 && factor != p2) {
				printf("Wrong: x: %llu, factor: %llu, p1: %llu, p2: %llu\n", x, factor, p1, p2);
			}
		}
		double run_time = omp_get_wtime() - start_time;
		printf("%d %f\n", NUM_THREADS, run_time);
	}

	delete [] integers;

	return 0;
}
