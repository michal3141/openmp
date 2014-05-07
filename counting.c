#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#define K 100
#define random() (rand() % K)

int n;
int thread_num;

void print_tab(int *tab, int n) // wypisywanie n elementow listy tab
{
	int i;
	printf("Array elements: \n");
	for (i = 0; i < n; i++)
	{
		printf("%d ", tab[i]);
	}
	printf("\n");
}
int* generate_random_nums(int n) // generowanie listy n losowych liczb typu int z przedzialu (0, K-1)
{
	int* ret = (int*) malloc(n * sizeof(int));
	int i;
	for (i = 0; i < n; i++)
	{
		ret[i] = random();
	}
	return ret;
}
int check_sort_validity(int *tab, int n) // sprawdzenie poprawnosci sortowania
{
	int i;
	for (i = 0; i < n-1; i++) 
	{
		if (tab[i] > tab[i+1]) 
		{
			printf("%d %d\n", tab[i], tab[i+1]);
			return 1;
		}
	}
	return 0;
} 

void countingsort(int* A, int* B, int n)
{
	int i;
	struct timeval start, end;
	double delta;
	int* C = (int*) malloc(K * sizeof(int));
	int* S = (int*) malloc(K * sizeof(int));
	
    gettimeofday(&start, NULL);

    #pragma omp parallel for shared(C) private(i)
	for (i = 0; i < K; i++)
		C[i] = 0;

	#pragma omp parallel
	{
		int i;
		int* C_private = (int*) malloc(K * sizeof(int));
		for (i = 0; i < K; i++)
			 C_private[i] = 0;

		#pragma omp for nowait
		for (i = 0; i < n; i++)
			C_private[A[i]]++;

		#pragma omp critical
		{
			for (i = 0; i < K; i++)
				C[i] += C_private[i];
		}
	}

	S[0] = 0;
	for (i = 1; i < K; i++)
	{
		C[i] = C[i] + C[i-1];
		S[i] = C[i-1] + 1;
	}

	#pragma omp parallel
	{
		int i;
		int id = omp_get_thread_num();
		int chunk = ceil((float) K / (float) thread_num);
		int start = id * chunk;
		int end = (id + 1) * chunk;
		if (end >= K) end = K - 1;
		for (i = start; i <= end; i++)
		{
			int j;
			int s = S[i];
			int e = C[i];
			for (j = s; j <= e; j++)
				B[j] = i;
		}
	}
	// print_tab(S, K);
	// print_tab(C, K);
	// #pragma omp parallel for shared(A, B, C) private(i)
	// for (i = 0; i < n; i++)
	// {
	// 	B[C[A[i]]] = A[i];
	// 	C[A[i]]--;
	// }

    gettimeofday(&end, NULL);
    delta = ((end.tv_sec - start.tv_sec) * 1000000u +
              end.tv_usec - start.tv_usec) / 1.e6;
    printf("%d %d %lf\n", thread_num, n, delta);
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	thread_num = atoi(argv[1]);
	n = atoi(argv[2]);

	omp_set_num_threads(thread_num);
	
	int *A = generate_random_nums(n);
	int *B = (int*) malloc(n * sizeof(int));
	int i;
	for (i = 0; i < n; i++)
		B[i] = 0;

	countingsort(A, B, n);
	// print_tab(B, n);
	int res = check_sort_validity(B, n);
	if (res)
		printf("Sorting does not work\n");

	return 0;
}