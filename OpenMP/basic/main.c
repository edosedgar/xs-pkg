#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <sys/syscall.h>

int
main(int argc, char* argv[])
{
	/* 1a Parallel sum */
	long int i = 0;
	long unsigned sum = 0;

	#pragma omp parallel for reduction(+:sum)
	for (i = 0; i < 100000; i++)
		sum += i;
	fprintf(stderr, "Result: %lu\n", sum);
	/* =============== */

	/* 1b Serial output */
	int N = 10;
	int flag = 1;
	omp_lock_t lock[N];

	for (i = 1; i < N; i++) {
		omp_init_lock(&lock[i]);
		omp_set_lock(&lock[i]);
	}

	#pragma omp parallel for
	for (i = 0; i < N * flag; i++) {
		if (i != 0)
			omp_set_lock(&lock[i]);
		fprintf(stderr, "%ld : %d\n", syscall(186), i);
		if (i == N - 1)
			flag = 0;
		else
			omp_unset_lock(&lock[i+1]);
	}

	for (i = 0; i < N; i++)
		omp_destroy_lock(&lock[i]);
	/* =============== */

	/* 1c.1 Min and Max with reduction */
	N = 120;
	srand(468);
	int vector[N];

	for (i = 0; i < N; i++)
		vector[i] = rand();
	
	int min = vector[0];
	int max = vector[0];

	#pragma omp parallel for reduction(max:max), reduction(min:min)
	for (i = 0; i < N; i++) {
		if (max < vector[i])
			max = vector[i];

		if (min > vector[i])
			min = vector[i];
	}
	fprintf(stderr, "max: %d min: %d\n", max, min);
	/* =============================== */

	/* 1c.2 Min and Max without reduction */
	omp_lock_t lock_one;
	omp_init_lock(&lock_one);
	min = vector[0];
	max = vector[0];

	#pragma omp parallel for
	for (i = 0; i < N; i++) {
		omp_set_lock(&lock_one);
		if (max < vector[i])
			max = vector[i];

		if (min > vector[i])
			min = vector[i];
		omp_unset_lock(&lock_one);
	}

	fprintf(stderr, "max: %d min: %d\n", max, min);
	/* =============================== */
	return 0;
}
