#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>

#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>


#define nano 1000000L

__global__ void dijkstras(int *a, int *b, int *n)
{
	int i;
	int d[10], p[10], v[10];
	// d stores distnce/cost of each path
	// p stores path taken
	// v stores the nodes already travelled to
	int k, u, s;
	int check = 0;

	// shared memory on cuda device
	__shared__ int temp[20];
	for (i = 0; i < (*n)*(*n); i++)
	{
		temp[i] = a[i];
	}
	check = check + 1;
	__syncthreads();

	// were passing int s -- node from which distances are calculated
	s = threadIdx.x;
	for (i = 0; i<(*n); i++)
	{
		d[i] = temp[s*(*n) + i];
		if (d[i] != 9999)
			p[i] = 1;
		else
			p[i] = 0;
		v[i] = 0;
	}
	p[s] = 0;
	v[s] = 1;
	for (i = 0; i<((*n) - 1); i++)
	{
		// findmin starts here
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
		// findmin ends here
		v[k] = 1;
		for (u = 0; u<(*n); u++)
		{
			if ((v[u] == 0) && (temp[k*(*n) + u] != 9999))
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



main()
{
	int *a, *b, *n;
	int *d_a, *d_b, *d_n;
	int i, j, c;
	int check = 0;
	printf("enter the number of vertices.... : ");
	n = (int*)malloc(sizeof(int));
	scanf("%d", n);
	int size = (*n) * (*n) * sizeof(int);

	//allocating device memory
	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);
	cudaMalloc((void **)&d_n, sizeof(int));

	a = (int*)malloc(size);
	b = (int*)malloc(size);

	
}