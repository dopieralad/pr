#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

constexpr int MIN_NUMBER = 0;
constexpr int MAX_NUMBER = 100;
constexpr int NUMBER_COUNT = MAX_NUMBER - MIN_NUMBER + 1;

int* initial_prime_numbers()
{
	int* prime_numbers = (int*) malloc(NUMBER_COUNT * sizeof(int));

	for (int number = 0; number < NUMBER_COUNT; number++)
	{
		prime_numbers[number] = 1;
	}

	return prime_numbers;
}

int is_within_range(int number)
{
	return MIN_NUMBER <= number && number <= MAX_NUMBER;
}

int index_for_number(int number)
{
	if (is_within_range(number))
	{
		return number - MIN_NUMBER;
	}
	else
	{
		return -1;
	}
}

int number_for_index(int index)
{
	if (0 <= index && index < NUMBER_COUNT)
	{
		return index + MIN_NUMBER;
	}
	else
	{
		return -1;
	}
}

void print_prime_numbers(int* prime_numbers)
{
	printf("Prime numbers are: ");

	for (int index = 0; index < NUMBER_COUNT; index++)
	{
		int is_prime = prime_numbers[index];
		if (is_prime)
		{
			printf("%d ", number_for_index(index));
		}
	}

	printf("\n");
}

void calculate_prime_numbers()
{
	int* prime_numbers = initial_prime_numbers();

	if (is_within_range(0)) {
		prime_numbers[index_for_number(0)] = 0;
	}

	if (is_within_range(1)) {
		prime_numbers[index_for_number(1)] = 0;
	}

	double max_divisor = sqrt(MAX_NUMBER);
	for (int number = 2; number <= max_divisor; number++)
	{
		for (int multiple = 2 * number; multiple <= MAX_NUMBER; multiple += number)
		{
			if (is_within_range(multiple))
			{
				prime_numbers[index_for_number(multiple)] = 0;
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
