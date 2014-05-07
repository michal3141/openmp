matvec: matvec.c
	gcc -Wall matvec.c -o matvec -fopenmp
counting: counting.c
	gcc -Wall counting.c -o counting -fopenmp -lm
matmul: matmul.c
	gcc -Wall matmul.c -o matmul -fopenmp -lm
bucket: bucket.c
	gcc -Wall bucket.c -o bucket -fopenmp -lm
yeval: yeval.f90
	gfortran yeval.f90 -o yeval -fopenmp
