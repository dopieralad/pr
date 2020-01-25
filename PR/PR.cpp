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
#include <windows.h>

// Type aliases
typedef signed long long big_int;

// Configuration
constexpr big_int OMP_THREADS = 8;
constexpr big_int MIN_NUMBER = 2;
constexpr big_int MAX_NUMBER = 1100000000;
constexpr big_int NUMBER_COUNT = MAX_NUMBER + 1;

bool* initial_prime_numbers(big_int number_count)
{
	bool* prime_numbers = (bool*)malloc(number_count * sizeof(bool)); // Allocate enough memory

	// Mark all numbers as primes initially
	for (big_int number = 0; number < number_count; number++)
	{
		prime_numbers[number] = true;
	}

	return prime_numbers;
}

void calculate_prime_numbers(bool* prime_numbers, big_int max_number)
{
	// Mark 0 and 1 as non-primes (by definition)
	prime_numbers[0] = false;
	prime_numbers[1] = false;

	// Calculate maximal divisor that may exclude non-primes
	big_int max_divisor = (big_int)ceil(sqrt(max_number));

	#pragma omp parallel shared(prime_numbers, max_divisor)
	{
		printf("Thread: '%d' has started!\n", omp_get_thread_num());

		// For each possible divisor
		#pragma omp for schedule(static) nowait
		for (big_int divisor = 2; divisor <= max_divisor; divisor++)
		{
			// If it is still considered prime (if not, then all its multiples are all already marked as non-primes also)
			if (prime_numbers[divisor] == true)
			{
				// Mark all multiples as non-primes
				for (big_int multiple = 2 * divisor; multiple <= max_number; multiple += divisor)
				{
					prime_numbers[multiple] = false;
				}
			}
		}

		printf("Thread: '%d' has finished!\n", omp_get_thread_num());
	}
}

void dump_prime_numbers(bool* prime_numbers, big_int min_number, big_int max_number, double elapsed_seconds, int with_primes)
{
	// Open an output file
	FILE* file;
	fopen_s(&file, "C:\\Users\\Daniel\\Desktop\\primes.txt", "a");

	// Count prime numbers
	big_int found = 0;
	for (big_int number = min_number; number <= max_number; number++)
	{
		bool is_prime = prime_numbers[number];
		if (is_prime == true)
		{
			found++;
		}
	}

	// Print summary line
	fprintf_s(file, "Min: '%lli' Max: '%lli' Found: '%lli' Elapsed: '%lf'\n", min_number, max_number, found, elapsed_seconds);

	if (with_primes)
	{
		// Print primes, 10 per each line
		int line_counter = 1;
		for (big_int number = min_number; number <= max_number; number++)
		{
			int is_prime = prime_numbers[number];
			if (is_prime)
			{
				fprintf_s(file, "%lli ", number);

				if (line_counter++ >= 10)
				{
					line_counter = 1;
					fprintf_s(file, "\n");
				}
			}
		}

		// End a file with a newline
		fprintf_s(file, "\n");
	}

	// Close the output file
	fclose(file);
}

void process(int omp_threads, big_int min_number, big_int max_number, big_int number_count)
{
	// Set number of OpenMP threads
	omp_set_num_threads(omp_threads);

	// Get initial prime number array
	bool* prime_numbers = initial_prime_numbers(number_count);

	// Measure prime number calculation time
	clock_t start = clock();
	calculate_prime_numbers(prime_numbers, max_number);
	clock_t stop = clock();

	// Print calculation time to the console
	double elapsed_seconds = ((stop - start) / 1000.0);
	printf("Calculations took: '%lf' seconds.\n", elapsed_seconds);

	// Dump prime numbers to the output file
	dump_prime_numbers(prime_numbers, min_number, max_number, elapsed_seconds, 0);

	// Cleanup
	delete prime_numbers;

	Sleep(1000);
}

int main()
{
	for (int omp_threads = 1; omp_threads <= 4; omp_threads++)
	{
		process(omp_threads, MIN_NUMBER, MAX_NUMBER, NUMBER_COUNT);
		process(omp_threads, MIN_NUMBER, MAX_NUMBER / 2, NUMBER_COUNT);
		process(omp_threads, MAX_NUMBER / 2, MAX_NUMBER, NUMBER_COUNT);
		process(omp_threads, MAX_NUMBER * 3 / 4, MAX_NUMBER, NUMBER_COUNT);
		Sleep(5000);
	}
}
