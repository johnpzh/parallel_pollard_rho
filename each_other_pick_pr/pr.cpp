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

	Ullint *ys = (Ullint *) malloc(sizeof(Ullint) * NUM_THREADS);
	Ullint x = (rand() % (n - 2)) + 2;
	for (int i = 0; i < NUM_THREADS; ++i) {
		ys[i] = x;
	}
	Ullint c = (rand() % (n - 1)) + 1;
	/* f() = x^2 + c mod n */
	auto f = [=](Ullint x) -> Ullint {
	   	return (modular_pow(x, 2, n) + c) % n;
	};
	int found = 0;
	Ullint result = n;

	omp_set_num_threads(NUM_THREADS);
	while (!found) {
		x = f(x);
#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		Ullint y = ys[tid];
		for (int i = 0; i < tid+2; ++i) {
			y = f(y);
		}
		ys[tid] = y;
	}
	// Compare x with ys.
#pragma omp parallel
		for (int i = 0; i < NUM_THREADS; ++i) {
			//if (found) {
			//	continue;
			//}
			Ullint d = __gcd((Ullint) llabs(ys[i] - x), n);
			if (d != 1 && !found) {
				found = 1;
				result = d;
			}
		}
		if (found) {
			break;
		}
	// Compare ys with each other.
#pragma omp parallel for
		for (int i = 0; i < NUM_THREADS; ++i) {
			if (found) {
				continue;
			}
			for (int j = i + 1; j < NUM_THREADS; ++j) {
				Ullint d = __gcd((Ullint) llabs(ys[j] - ys[i]), n);
				if (d != 1 && !found) {
					found = 1;
					result = d;
					break;
				}
				if (found) {
					break;
				}
			}
		}
		if (!found) {
			Ullint y = ys[NUM_THREADS - 1];
#pragma omp parallel for
			for (int i = 0; i < NUM_THREADS; ++i) {
				ys[i] = y;
			}
		}
	}
	
	free(ys);
	if (result == n) {
		return 0;
	} else {
		return result;
	}
}

int main(int argc, char *argv[])
{
	const int len = 10000;
	//const int len = 100000;
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
	//printf("input finished.\n");//test

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
			//Ullint p1 = std::get<1>(integers[i]);
			//Ullint p2 = std::get<2>(integers[i]);
			//if (factor != p1 && factor != p2) {
			//	printf("Wrong: x: %llu, factor: %llu, p1: %llu, p2: %llu\n", x, factor, p1, p2);
			//}
		}
		double run_time = omp_get_wtime() - start_time;
		printf("%d %f\n", NUM_THREADS, run_time);
	}

	delete [] integers;

	return 0;
}
