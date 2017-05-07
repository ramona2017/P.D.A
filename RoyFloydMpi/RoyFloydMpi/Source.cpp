#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <mpi.h>

const int INFINITY = 1000000;

void Read_matrix(int local_mat[], int n, int my_rank, int p,
	MPI_Comm comm);
void Print_matrix(int local_mat[], int n, int my_rank, int p,
	MPI_Comm comm);
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);
int Owner(int k, int p, int n);
void Copy_row(int local_mat[], int n, int p, int row_k[], int k);
void Print_row(int local_mat[], int n, int my_rank, int i);

int main(int argc, char* argv[]) {
	int  n;
	int* local_mat;
	MPI_Comm comm;
	int p, my_rank;

	MPI_Init(&argc, &argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &my_rank);

	if (my_rank == 0) {
		printf("How many vertices?\n");
		scanf_s("%d", &n);
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, comm);
	local_mat = (int*)malloc(n*n / p * sizeof(int));

	if (my_rank == 0) printf("Enter the local_matrix\n");
	Read_matrix(local_mat, n, my_rank, p, comm);
	if (my_rank == 0) printf("We got\n");
	Print_matrix(local_mat, n, my_rank, p, comm);
	if (my_rank == 0) printf("\n");

	Floyd(local_mat, n, my_rank, p, comm);

	if (my_rank == 0) printf("The solution is:\n");
	Print_matrix(local_mat, n, my_rank, p, comm);

	free(local_mat);
	MPI_Finalize();

	return 0;
}


void Read_matrix(int local_mat[], int n, int my_rank, int p,                                                                    /*Read in the local_matrix on process 0 and scatter it using a block row distribution among the processes.*/
	MPI_Comm comm) {  
	int i, j;
	int* temp_mat = NULL;

	if (my_rank == 0) {
		temp_mat = (int*)malloc(n*n * sizeof(int));
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				scanf_s("%d", &temp_mat[i*n + j]);
		MPI_Scatter(temp_mat, n*n / p, MPI_INT,
			local_mat, n*n / p, MPI_INT, 0, comm);
		free(temp_mat);
	}
	else {
		MPI_Scatter(temp_mat, n*n / p, MPI_INT,
			local_mat, n*n / p, MPI_INT, 0, comm);
	}

}

void Print_row(int local_mat[], int n, int my_rank, int i) {                                                               /*Convert a row of the matrix to a string and then print the string.*/
	char char_int[100];
	char char_row[1000];
	int j, offset = 0;

	for (j = 0; j < n; j++) {
		if (local_mat[i*n + j] == INFINITY)
			sprintf_s(char_int, "i ");
		else
			sprintf_s(char_int, "%d ", local_mat[i*n + j]);
		sprintf(char_row + offset, "%s", char_int);
		offset += strlen(char_int);
	}
	printf("Proc %d > row %d = %s\n", my_rank, i, char_row);
}

//   Gather the distributed matrix into process 0 and print it.

void Print_matrix(int local_mat[], int n, int my_rank, int p,
	MPI_Comm comm) {
	int i, j;
	int* temp_mat = NULL;

	if (my_rank == 0) {
		temp_mat = (int*)malloc(n*n * sizeof(int));
		MPI_Gather(local_mat, n*n / p, MPI_INT,
			temp_mat, n*n / p, MPI_INT, 0, comm);
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++)
				if (temp_mat[i*n + j] == INFINITY)
					printf("i ");
				else
					printf("%d ", temp_mat[i*n + j]);
			printf("\n");
		}
		free(temp_mat);
	}
	else {
		MPI_Gather(local_mat, n*n / p, MPI_INT,
			temp_mat, n*n / p, MPI_INT, 0, comm);
	}
}

void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) {
	int global_k, local_i, global_j, temp;
	int root;
	int* row_k = (int*)malloc(n * sizeof(int));

	for (global_k = 0; global_k < n; global_k++) {
		root = Owner(global_k, p, n);
		if (my_rank == root)
			Copy_row(local_mat, n, p, row_k, global_k);
		MPI_Bcast(row_k, n, MPI_INT, root, comm);
		for (local_i = 0; local_i < n / p; local_i++)
			for (global_j = 0; global_j < n; global_j++) {
				temp = local_mat[local_i*n + global_k] + row_k[global_j];
				if (temp < local_mat[local_i*n + global_j])
					local_mat[local_i*n + global_j] = temp;
			}
	}
	free(row_k);
}  



int Owner(int k, int p, int n) { //:   Return rank of process that owns global row k
	return k / (n / p);
} 

  
void Copy_row(int local_mat[], int n, int p, int row_k[], int k) {                                                          //   Copy the row with *global* subscript k into row_k

	int j;
	int local_k = k % (n / p);

	for (j = 0; j < n; j++)
		row_k[j] = local_mat[local_k*n + j];

}

