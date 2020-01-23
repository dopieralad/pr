#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef unsigned long long big_int;
typedef char boolean;

constexpr big_int MIN_NUMBER = 0;
constexpr big_int MAX_NUMBER = 1000000000;
constexpr big_int NUMBER_COUNT = MAX_NUMBER + 1;

constexpr char TRUE = 1;
constexpr char FALSE = 0;

boolean* initial_prime_numbers()
{
	boolean* prime_numbers = (boolean*)malloc(NUMBER_COUNT * sizeof(boolean));

	for (big_int number = 0; number < NUMBER_COUNT; number++)
	{
		prime_numbers[number] = TRUE;
	}

	return prime_numbers;
}

void print_prime_numbers(boolean* prime_numbers)
{
	FILE* file;
	fopen_s(&file, ".\\primes.txt", "w");

	big_int found = 0;
	for (big_int number = MIN_NUMBER; number <= MAX_NUMBER; number++)
	{
		int is_prime = prime_numbers[number];
		if (is_prime == TRUE)
		{
			found++;
		}
	}

	fprintf_s(file, "Min: '%llu' Max: '%llu' Found: '%llu'\n", MIN_NUMBER, MAX_NUMBER, found);

	int line_counter = 0;
	for (big_int number = MIN_NUMBER; number <= MAX_NUMBER; number++)
	{
		int is_prime = prime_numbers[number];
		if (is_prime)
		{
			fprintf_s(file, "%llu ", number);

			if (line_counter++ >= 10)
			{
				line_counter = 0;
				fprintf_s(file, "\n");
			}
		}
	}

	fprintf_s(file, "\n");

	fclose(file);
}

boolean* calculate_prime_numbers()
{
	boolean* prime_numbers = initial_prime_numbers();

	prime_numbers[0] = FALSE;
	prime_numbers[1] = FALSE;

	long double max_divisor = sqrt(MAX_NUMBER);
	for (big_int divisor = 2; divisor <= max_divisor; divisor++)
	{
		if (prime_numbers[divisor] == TRUE) // Divisor is still considered prime
		{
			for (big_int multiple = 2 * divisor; multiple <= MAX_NUMBER; multiple += divisor)
			{
				prime_numbers[multiple] = FALSE;
			}
		}
	}

	return prime_numbers;
}

void measure_prime_calculations()
{
	clock_t start = clock();
	boolean* prime_numbers = calculate_prime_numbers();
	clock_t stop = clock();

	double elapsed_seconds = ((stop - start) / 1000.0);
	printf("Calculations took: '%lf' seconds.", elapsed_seconds);

	print_prime_numbers(prime_numbers);
	delete prime_numbers;
}

int main()
{
	measure_prime_calculations();
}
