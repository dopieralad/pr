#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

constexpr int MIN_NUMBER = 0;
constexpr int MAX_NUMBER = 100;
constexpr int NUMBER_COUNT = MAX_NUMBER + 1;

int* initial_prime_numbers()
{
	int* prime_numbers = (int*) malloc(NUMBER_COUNT * sizeof(int));

	for (int number = 0; number < NUMBER_COUNT; number++)
	{
		prime_numbers[number] = 1;
	}

	return prime_numbers;
}

void print_prime_numbers(int* prime_numbers)
{
	printf("Prime numbers are: ");

	for (int number = MIN_NUMBER; number <= MAX_NUMBER; number++)
	{
		int is_prime = prime_numbers[number];
		if (is_prime)
		{
			printf("%d ", number);
		}
	}

	printf("\n");
}

void calculate_prime_numbers()
{
	int* prime_numbers = initial_prime_numbers();

	prime_numbers[0] = 0;
	prime_numbers[1] = 0;

	double max_divisor = sqrt(MAX_NUMBER);
	for (int divisor = 2; divisor <= max_divisor; divisor++)
	{
		if (prime_numbers[divisor] == 1) // Divisor is still considered prime
		{
			for (int multiple = 2 * divisor; multiple <= MAX_NUMBER; multiple += divisor)
			{
				prime_numbers[multiple] = 0;
			}
		}
	}

	print_prime_numbers(prime_numbers);

	delete prime_numbers;
}

void measure(void (*procedure)())
{
	clock_t start = clock();
	procedure();
	clock_t stop = clock();

	double elapsed_seconds = ((stop - start) / 1000.0);
	printf("Calculations took: '%lf' seconds.", elapsed_seconds);
}

int main()
{
	measure(calculate_prime_numbers);
}
