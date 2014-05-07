#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>

#define random() ((float) rand() / (RAND_MAX))

int n;
int thread_num;

void print_tab(float *tab, int n) // wypisywanie n elementow listy tab
{
	int i;
	printf("Array elements: \n");
	for (i = 0; i < n; i++)
	{
		printf("%f ", tab[i]);
	}
	printf("\n");
}
float* generate_random_nums(int n) // generowanie listy n losowych liczb typu int z przedzialu (0, K-1)
{
	float* ret = (float*) malloc(n * sizeof(float));
	int i;
	for (i = 0; i < n; i++)
	{
		ret[i] = random();
	}
	return ret;
}
int check_sort_validity(float *tab, int n) // sprawdzenie poprawnosci sortowania
{
	int i;
	for (i = 0; i < n-1; i++) 
	{
		if (tab[i] > tab[i+1]) 
		{
			printf("%f %f\n", tab[i], tab[i+1]);
			return 1;
		}
	}
	return 0;
} 

int float_sorter( const void *first_arg, const void *second_arg )
{
    float first = *(float*)first_arg;
    float second = *(float*)second_arg;
    if ( first < second )
    {
        return -1;
    }
    else if ( first == second )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	thread_num = atoi(argv[1]);
	n = atoi(argv[2]);

	omp_set_num_threads(thread_num);
	
	float *A = generate_random_nums(n);
	float *B = (float*) malloc(n * sizeof(float));
	int i;
	for (i = 0; i < n; i++)
		B[i] = 0;

	qsort(A, n, sizeof(float), float_sorter);

	//countingsort(A, B, n);
	print_tab(A, n);
	int res = check_sort_validity(B, n);
	if (res)
		printf("Sorting does not work\n");

	
	return 0;
}

