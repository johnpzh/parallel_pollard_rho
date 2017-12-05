#include <cstdio>
#include <cstdlib>

void combine()
{
	FILE *fin1 = fopen("primes1.txt", "r");
	FILE *fin2 = fopen("primes50.txt", "r");
	FILE *fout = fopen("integers.txt", "w");

	if (fin1 == nullptr) {
		printf("Error: cannot open fin1.\n");
		exit(1);
	}
	if (fin2 == nullptr) {
		printf("Error: cannot open fin2.\n");
		exit(1);
	}
	if (fout == nullptr) {
		printf("Error: cannot open fout.\n");
		exit(1);
	}

	/* Input data
	*/
	const int total = 1000000;
	const int length = 100;
	for (int i = 0; i < total - length; ++i) {
		fscanf(fin2, "%*llu");
	}
	unsigned long long p1s[length];
	unsigned long long p2s[length];

	for (int i = 0; i < length; ++i) {
		unsigned long long p1;
		unsigned long long p2;
		fscanf(fin1, "%llu", &p1);
		fscanf(fin2, "%llu", &p2);
		p1s[i] = p1;
		p2s[i] = p2;
	}

	/* Output data
	   */
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			unsigned long long p1 = p1s[i];
			unsigned long long p2 = p2s[j];
			unsigned long long big = p1 * p2;
			fprintf(fout, "%llu %llu %llu\n", big, p1, p2);
		}
	}

	fclose(fout);
	fclose(fin2);
	fclose(fin1);
}

int main()
{
	combine();

	return 0;
}

