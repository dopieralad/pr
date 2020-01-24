/*
* A Visual Studio pre-compiled header file.
* This has to be at the top, because everything
* that comes before it is discarded.
*/
#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

// Type aliases
typedef signed long long big_int;

// Configuration
constexpr big_int OMP_THREADS = 8;
constexpr big_int MIN_NUMBER = 0;
constexpr big_int MAX_NUMBER = 100000000;
constexpr big_int NUMBER_COUNT = MAX_NUMBER + 1;

bool* initial_prime_numbers()
{
	bool* prime_numbers = (bool*)malloc(NUMBER_COUNT * sizeof(bool)); // Allocate enough memory

	// Mark all numbers as primes initially
	for (big_int number = 0; number < NUMBER_COUNT; number++)
	{
		prime_numbers[number] = true;
	}

	return prime_numbers;
}

void calculate_prime_numbers(bool* prime_numbers)
{
	// Mark 0 and 1 as non-primes (by definition)
	prime_numbers[0] = false;
	prime_numbers[1] = false;

	// Calculate maximal divisor that may exclude non-primes
	big_int max_divisor = (big_int)ceil(sqrt(MAX_NUMBER));

	// For each possible divisor
	#pragma omp parallel for shared(prime_numbers, max_divisor) schedule(dynamic)
	for (big_int divisor = 2; divisor <= max_divisor; divisor++)
	{
		// If it is still considered prime (if not, then all its multiples all already marked as non-primes also)
		if (prime_numbers[divisor] == true)
		{
			// Mark all multiples as non-primes
			for (big_int multiple = 2 * divisor; multiple <= MAX_NUMBER; multiple += divisor)
			{
				prime_numbers[multiple] = false;
			}
		}
	}
}

void dump_prime_numbers(bool* prime_numbers)
{
	// Open an output file
	FILE* file;
	fopen_s(&file, "C:\\Users\\Daniel\\Desktop\\primes.txt", "w");

	// Count prime numbers
	big_int found = 0;
	for (big_int number = MIN_NUMBER; number <= MAX_NUMBER; number++)
	{
		bool is_prime = prime_numbers[number];
		if (is_prime == true)
		{
			found++;
		}
	}

	// Print summary line
	fprintf_s(file, "Min: '%lli' Max: '%lli' Found: '%lli'\n", MIN_NUMBER, MAX_NUMBER, found);

	// Print primes, 10 per each line
	int line_counter = 0;
	for (big_int number = MIN_NUMBER; number <= MAX_NUMBER; number++)
	{
		int is_prime = prime_numbers[number];
		if (is_prime)
		{
			fprintf_s(file, "%lli ", number);

			if (line_counter++ >= 10)
			{
				line_counter = 0;
				fprintf_s(file, "\n");
			}
		}
	}

	// End a file with a newline
	fprintf_s(file, "\n");

	// Close the output file
	fclose(file);
}

int main()
{
	// Set number of OpenMP threads
	omp_set_num_threads(OMP_THREADS);

	// Get initial prime number array
	bool* prime_numbers = initial_prime_numbers();

	// Measure prime number calculation time
	clock_t start = clock();
	calculate_prime_numbers(prime_numbers);
	clock_t stop = clock();

	// Print calculation time to the console
	double elapsed_seconds = ((stop - start) / 1000.0);
	printf("Calculations took: '%lf' seconds.", elapsed_seconds);

	// Dump prime numbers to the output file
	dump_prime_numbers(prime_numbers);

	// Cleanup
	delete prime_numbers;
}
