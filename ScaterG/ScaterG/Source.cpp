#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<time.h>
#define SIZE 100

using namespace std;

int main(int argc, char **argv) {
	int rank, size, segmentSize, startPos, endPos, index;
	int arr[SIZE];
	int KEY = 20;
	int maxPosition = 0;
	int max = 0;

	srand(time(NULL) + 1);

	for (index = 0; index < SIZE; index++) {
		arr[i] = rand() % 100;
	}
	srand(time(NULL) + 1);

	MPI_Status status;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	segmentSize = SIZE / size;
	if (SIZE % size != 0)
		++segmentSize;

	startPos = rank * segmentSize;
	endPos = startPos + segmentSize - 1;

	startPos = rank * segmentSize;
	endPos = startPos + segmentSize - 1;

	MPI_Bcast(arr, 10, MPI_INT, 0, MPI_COMM_WORLD);
	for (index = startPos; index <= endPos; index++) {
		printf("%d ", arr[i]);
		if (arr[i] == KEY) {
			max = index;
		}
	}


	MPI_Reduce(&max, &maxPosition, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("%d max position: ", maxPosition);
	}

	MPI_Finalize();

	return 0;
}