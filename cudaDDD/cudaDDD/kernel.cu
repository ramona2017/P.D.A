#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#ifndef __CUDACC__ 
#define __CUDACC__
#endif
#include <cuda.h>
#include <device_functions.h>
#include <cuda_runtime_api.h>
#include<time.h>
#include <stdio.h>

#define nano 1000000L

__global__ void dijkstras(int *a, int *b, int *n)
{
	int i;
	int d[10], p[10], v[10];// d este costul fiecarei cai ;                     p :Stochează calea luată; v Stochează nodurile deja vizitate
	int k, u, s, check = 0;

	__shared__ int temp[20];
	for (i = 0; i < (*n)*(*n); i++)
	{
		temp[i] = a[i];
	}
	check = check + 1;
	__syncthreads();

	s = threadIdx.x;
	for (i = 0; i<(*n); i++)
	{
		d[i] = temp[s*(*n) + i];
		if (d[i] != 999)
			p[i] = 1;
		else
			p[i] = 0;
		v[i] = 0;
	}
	p[s] = 0;
	v[s] = 1;

	for (i = 0; i<((*n) - 1); i++)
	{

		int i1, j1, min = 0;
		for (i1 = 0; i1<(*n); i1++)
		{
			if (v[i1] == 0)
			{
				min = i1;
				break;
			}
		}
		for (j1 = min + 1; j1<(*n); j1++)
		{
			if ((v[j1] == 0) && (d[j1]<d[min]))
				min = j1;
		}
		k = min;

		v[k] = 1;
		for (u = 0; u<(*n); u++)
		{
			if ((v[u] == 0) && (temp[k*(*n) + u] != 999))
			{
				if (d[u]>d[k] + temp[k*(*n) + u])
				{
					d[u] = d[k] + temp[k*(*n) + u];
					p[u] = k;
				}
			}
		}
		//storing output
		int count = 0;
		for (i = (s*(*n)); i< (s + 1) * (*n); i++)
		{
			b[i] = d[count];
			count++;
		}
	}
	*n = check;
}




void main()
{
	int *a, *b, *n;
	int *d_a, *d_b, *d_n;
	int i, j, c;
	int check = 0;
	printf("Introduceți numărul de noduri.... : ");
	n = (int*)malloc(sizeof(int));
	scanf("%d", n);
	int size = (*n) * (*n) * sizeof(int);

	//allocating device memory
	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);
	cudaMalloc((void **)&d_n, sizeof(int));

	a = (int*)malloc(size);
	b = (int*)malloc(size);

	check = check + 1;
	for (i = 0; i<(*n); i++)
		for (j = 0; j <= i; j++)
			if (i == j)
				a[(i*(*n) + j)] = 0;
			else
				a[(i*(*n) + j)] = a[(j*(*n) + i)] = 999;

	printf("\nMatricea initiala: \n");
	for (i = 0; i<(*n); i++)
	{
		for (j = 0; j<(*n); j++)
		{
			printf("%d ", a[i*(*n) + j]);
		}
		printf("\n");
	}

	while (1)
	{
		printf("\n Introduceti nodul sursa, nodul destinatie si costul  \n");
		scanf("%d %d %d", &i, &j, &c);
		if (i == -1)
			break;
		a[(i*(*n) + j)] = a[(j*(*n) + i)] = c;
	}

	printf("\n \n");
	for (i = 0; i<(*n); i++)
	{
		for (j = 0; j<(*n); j++)
		{
			printf("%d ", a[i*(*n) + j]);
		}
		printf("\n");
	}

	check = check + 1;
	// copying input matrix to device
	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_n, n, sizeof(int), cudaMemcpyHostToDevice);
	check++;

	int N = *n;

	dijkstras <<< 1, N >>>(d_a, d_b, d_n);
	check++;

	// copying result from device to host
	cudaMemcpy(b, d_b, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(n, d_n, sizeof(int), cudaMemcpyDeviceToHost);

	check++;
	//  printing result
	printf("Cele mai scurte căi sunt....");
	for (i = 0; i<(N); i++)
	{
		for (j = 0; j<(N); j++)
		{
			if (i != j)
				printf("\n Costul căii de la %d la %d = %d\n", i, j, b[i*(N)+j]);
		}
		printf("\n\n");
	}

	free(a);
	free(b);
	free(n);
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_n);
}

