
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int size, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/*int *globaldata = NULL;
	int localdata;*/
	int n = 10, i = 0, x = 4, gasit = 0;
	int a[10] = { 3,6,0,5,8,4,2 };

	while ((i<n) && (gasit == 0))
		if (a[i] == x)
			gasit = 1;
		else
			i++;

	MPI_Scatter(a, 1, MPI_INT, &x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (gasit == 1)
	{

		printf("Numarul %d se afla in vector pe pozitia a  %d \n",
			x, i + 1);
	}

	else {
		printf("Not found");
	}


	MPI_Gather(&x, 1, MPI_INT, a, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("Processor %d has data: ", rank);
		for (int i = 0; i<size; i++)
			printf("%d ", a[i]);
		printf("\n");
	}

	

	MPI_Finalize();
	return 0;
}
/*while ((i<n) && (gasit == 0))
	if (a[i] == x)
		gasit = 1;
	else
		i++;
//se afiseaza rezultatele
if (gasit == 1)
{

	printf("Numarul %d se afla in vector pe pozitia a  %d \n",
		x, i + 1);
}

else {
	printf("Not found");
}
int n = 10, i = 0, x = 4, gasit = 0;
int a[100] = { 3,6,0,5,8,4,2 };*/
