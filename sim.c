#include <stdio.h>

long long int time = 0;

typedef struct e
{
	long long int t; // time of event
	int valid; // 1 if valid, 0 if invalid
	int b1, b2; // index of balls involved in the collision
} event;

typedef struct b
{
	int color;

	// position
	float x;
	float y;

	// velocity
	float vx;
	float vy;

	// linked list events
	event *coll;
} ball;

// we need to create a heap of events

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

void swap(event Q[], int i, int j)
{
	event temp = Q[j];
	Q[j] = Q[i];
	Q[i] = temp;
}

void heapify(event Q[], int heap_size, int i)
{
	int l = left(i);
	int r = right(i);
	
	int minimum = i;
	
	if(l<heap_size)
	{
		if(Q[l].t<Q[i].t)
			minimum = l;
		else minimum = i;
	}	
	
	if(r<heap_size)
	{
		if(Q[r].t<Q[minimum].t)
			minimum = r;
	}
	
	
	if( minimum != i)
	{
		swap(Q, i, minimum);
		heapify(Q, heap_size, minimum);
	}
}

void heap_insert(event Q[], int *heap_size, event k)
{
	*heap_size += 1;
	
	int i = (*(heap_size)) - 1;
	
	while(i>0 && Q[parent(i)].t > k.t)
	{
		Q[i] = Q[parent(i)];
		i = parent(i);
	}
	
	Q[i] = k;
}

event extract_min(event Q[], int *heap_size)
{
	if(*heap_size < 0)
	{
		event temp;
		temp.t = -1;
		temp.valid = -1;
		temp.b1 = -1;
		temp.b2 = -1;
		return temp;
	}
	event min = Q[0];
	Q[0] = Q[(*heap_size) - 1];
	*heap_size = *heap_size - 1;
	heapify(Q, *heap_size, 0);
	return min;
}

void PQ_insert(event Q[], int* q_size, event k)
{
	heap_insert(Q, q_size, k);
}

event PQ_extract(event Q[], int* q_size)
{
	return extract_min(Q, q_size);
}


void main()
{
	event q[10];
	int size = 0, i=0;

	for(i = 10; i>=1; i--)
	{
		event temp;
		temp.t = i;
		temp.valid = 1;
		temp.b1 = i;
		temp.b2 = i-1;

		PQ_insert(q, &size, temp);
	}

	for(i = 0; i<size; i++)
		printf("%lld\t", q[i].t);

	printf("\n");

	event temp = PQ_extract(q, &size);
	printf("%d\n", temp.b2);

	for(i = 0; i<size; i++)
		printf("%lld\t", q[i].t);

	printf("\n");


}
