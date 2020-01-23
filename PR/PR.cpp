#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef unsigned long long big_int;
constexpr big_int MIN_NUMBER = 0;
constexpr big_int MAX_NUMBER = 100;
constexpr big_int NUMBER_COUNT = MAX_NUMBER + 1;

big_int* initial_prime_numbers()
{
	big_int* prime_numbers = (big_int*)malloc(NUMBER_COUNT * sizeof(big_int));

	for (big_int number = 0; number < NUMBER_COUNT; number++)
	{
		prime_numbers[number] = 1;
	}

	return prime_numbers;
}

void print_prime_numbers(big_int* prime_numbers)
{
	FILE* file;
	fopen_s(&file, ".\\primes.txt", "w");

	big_int found = 0;
	for (big_int number = MIN_NUMBER; number <= MAX_NUMBER; number++)
	{
		int is_prime = prime_numbers[number];
		if (is_prime)
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

big_int* calculate_prime_numbers()
{
	big_int* prime_numbers = initial_prime_numbers();

	prime_numbers[0] = 0;
	prime_numbers[1] = 0;

	long double max_divisor = sqrt(MAX_NUMBER);
	for (big_int divisor = 2; divisor <= max_divisor; divisor++)
	{
		if (prime_numbers[divisor] == 1) // Divisor is still considered prime
		{
			for (big_int multiple = 2 * divisor; multiple <= MAX_NUMBER; multiple += divisor)
			{
				prime_numbers[multiple] = 0;
			}
		}
	}

	return prime_numbers;
}

void measure_prime_calculations()
{
	clock_t start = clock();
	big_int* prime_numbers = calculate_prime_numbers();
	clock_t stop = clock();

	print_prime_numbers(prime_numbers);
	delete prime_numbers;

	double elapsed_seconds = ((stop - start) / 1000.0);
	printf("Calculations took: '%lf' seconds.", elapsed_seconds);
}

int main()
{
	measure_prime_calculations();
}
