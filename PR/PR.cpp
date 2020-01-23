#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr int NUMBER_COUNT = 1000;

int* initial_prime_numbers(int size)
{
	int* prime_numbers = (int*) malloc(size * sizeof(int));

	for (int number = 0; number < size; number++)
	{
		prime_numbers[number] = 1;
	}

	prime_numbers[0] = 0;
	prime_numbers[1] = 0;

	return prime_numbers;
}

void print_prime_numbers(int* prime_numbers, int size)
{
	printf("Prime numbers are: ");

	for (int number = 0; number < size; number++)
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
	int* prime_numbers = initial_prime_numbers(NUMBER_COUNT + 1);


	for (int number = 2; number <= NUMBER_COUNT; number++)
	{
		for (int multiple = 2 * number; multiple <= NUMBER_COUNT; multiple = multiple + number)
		{
			prime_numbers[multiple] = 0;
		}
	}

	print_prime_numbers(prime_numbers, NUMBER_COUNT);

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
