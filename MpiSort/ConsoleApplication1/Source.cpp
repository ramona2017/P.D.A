#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define MASTER 0
int main(int argc, char *argv[])
{
	int n = 10, i = 0, x = 4, gasit = 0;
	int a[100] = { 3,6,0,5,8,4,2 };
	int numprocs, procid, len;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int partner, message;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	MPI_Get_processor_name(hostname, &len);
	
	// se cauta valoarea lui x printre elementele vectorului 
	while ((i<n) && (gasit == 0))
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


	if (procid == MASTER)
		printf("/nMASTER: Number of MPI procs is: %d\n", numprocs);
	/* determine partner and then send/receive with partner */
	if (procid < numprocs / 2) {
		partner = numprocs / 2 + procid;
		MPI_Send(&procid, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
		MPI_Recv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
	}
		else if (procid >= numprocs / 2) {
		partner = procid - numprocs / 2;
		MPI_Recv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
		MPI_Send(&procid, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
	} 
		/*print partner info and exit*/

		printf("\nProc %d is partner with %d\n", procid, message);
	MPI_Finalize();
}