#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#define random() ((double) rand() / (RAND_MAX))

void printMat(double** mati, int size) {
    int i,j;
    for (i = 0; i < size; i++) {        
        for (j = 0; j < size; j++) {
            printf("%lf ", mati[i][j]);
        }
        printf("\n");
    }
}
void printVec(double* vec, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%lf ", vec[i]);
    }
    printf("\n");
}

int size, thread_num;
int main(int argc, char* argv[])
{
    size = atoi(argv[2]);
    thread_num = atoi(argv[1]);
    omp_set_num_threads(thread_num);
    srand(time(NULL));
	
    double* b = (double*) malloc(size * sizeof(double));
    double* c = (double*) malloc(size * sizeof(double));

    double** a = (double**) malloc(size * sizeof(double*));
    int i, j;
    for (i = 0; i < size; i++)
        a[i] = (double*) malloc(size * sizeof(double)); 

    for (i = 0; i < size; ++i) {
        b[i] = random();
        c[i] = 0.0;
        for (j = 0; j < size; ++j) {
            a[i][j] = random();
        }
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    #pragma omp parallel for shared(a,b,c) private(i, j)
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            c[i] += a[i][j] * b[j];
        }
    }
  
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u +
              end.tv_usec - start.tv_usec) / 1.e6;
    printf("%d %d %lf\n", thread_num, size, delta);
    //printMat(a, size);
    //printf("---------------------------------\n");
    //printVec(b, size);
    //printf("---------------------------------\n");
    //printVec(c, size);
    //printf("---------------------------------\n");
    return 0;
}