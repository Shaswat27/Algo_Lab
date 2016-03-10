#include <stdio.h>

int parent(int i)
{
	if(i%2 == 0)
		return i/2 -1;
	return i/2;
	
}

int left(int i)
{
	return 2*i + 1;
}

int right(int i)
{
	return 2*i + 2;
}

void swap(int arr[], int i, int j)
{
	int temp = arr[j];
	arr[j] = arr[i];
	arr[i] = temp;
}

void heapify(int arr[], int heap_size, int i)
{
	int l = left(i);
	int r = right(i);
	
	int minimum = i;
	
	if(l<heap_size)
	{
		if(arr[l]<arr[i])
			minimum = l;
		else minimum = i;
	}	
	
	if(r<heap_size)
	{
		if(arr[r]<arr[minimum])
			minimum = r;
	}
	
	
	if( minimum != i)
	{
		swap(arr, i, minimum);
		heapify(arr, heap_size, minimum);
	}
}

void create_heap(int arr[], int heap_size)
{
	int i = heap_size/2;
	
	for(; i>=0; i--)
		heapify(arr, heap_size, i);
}

void heap_insert(int arr[], int *heap_size, int k)
{
	*heap_size += 1;
	
	int i = (*(heap_size)) - 1;
	
	while(i>0 && arr[parent(i)] > k)
	{
		arr[i] = arr[parent(i)];
		i = parent(i);
	}
	
	arr[i] = k;
}

int extract_min(int arr[], int *heap_size)
{
	if(*heap_size < 0)
		return -1;
	int min = arr[0];
	arr[0] = arr[(*heap_size) - 1];
	*heap_size = *heap_size - 1;
	heapify(arr, *heap_size, 0);
	return min;
}

void PQ_insert(int Q[], int* q_size, int k)
{
	heap_insert(Q, q_size, k);
}

int PQ_extract(int Q[], int* q_size)
{
	return extract_min(Q, q_size);
}


void main()
{
	int q[10];
	int size = 0, i = 0;

	for(i = 10; i>=1; i--)
		PQ_insert(q, &size, i);

	for(i = 0; i<size; i++)
		printf("%d\t", q[i]);

	printf("\n");

	printf("%d\n", PQ_extract(q, &size));

	for(i = 0; i<size; i++)
		printf("%d\t", q[i]);

	printf("\n");

	printf("%d\n", PQ_extract(q, &size));

	for(i = 0; i<size; i++)
		printf("%d\t", q[i]);

	printf("\n");

	printf("%d\n", PQ_extract(q, &size));
		
}
