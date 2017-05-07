#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"


#define N 4     

void display(char *result, int a[N][N])
{
	int i, j;

	printf("\n\n%s\n", result);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf(" %d", a[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	int i, j, rank, size, sum = 0;
	int a[N][N] = { { 1,7,2,3 },{ 1,8,3,4 },{ 1,9,4,5 },{ 1,9,4,6 } };
	int b[N][N] = { { 2,3,4 ,2},{ 3,4,5,4 },{ 4,5,6,6 } ,{ 1,9,4 ,1} };
	int c[N][N];
	int aa[N], cc[N];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
	MPI_Scatter(a, N*N / size, MPI_INT, aa, N*N / size, MPI_INT, 0, MPI_COMM_WORLD);

	//broadcast second matrix to all processes
	MPI_Bcast(b, N*N, MPI_INT, 0, MPI_COMM_WORLD);

	
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			sum = sum + aa[j] * b[j][i];
			
		}

		cc[i] = sum;

		sum = 0;
	}

	MPI_Gather(cc, N*N / size, MPI_INT, c, N*N / size, MPI_INT, 0, MPI_COMM_WORLD);


	if (rank == 0)
		display(" Matricea rezultat", c);

	MPI_Finalize();
	
	exit(0);

}

