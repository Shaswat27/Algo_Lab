#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double time = 0;

typedef struct e
{
	double t; // time of event
	int valid; // 1 if valid, 0 if invalid
	int b1, b2; // index of balls involved in the collision
} event;

typedef struct l
{
	event *evt;
	struct l *next;
} list;

typedef struct b
{
	int color;

	// radius
	double radius;

	// position
	double x;
	double y;

	// velocity
	double vx;
	double vy;

	// linked list events
	list *coll;
} ball;

event PQ[100];
int PQ_size = 0;
ball b[5];

// insert an event into the list 
void list_insert(ball* b, event* et)
{
	list **head = &(b->coll);
	list *p = (*head);

	ball bl = *b;
	//printf("Ball's vx: %lf\n", bl.vx);

	if (*head == NULL)
	{
		list *temp = (list *)malloc(sizeof(list));
		temp->evt = et;
		temp->next = NULL;
		*head = temp;
	}

	else
	{
		while(p->next != NULL)
		{
			p = p->next;
		}

		list *temp = (list *)malloc(sizeof(list));
		temp->evt = et;
		temp->next = NULL;

		p->next = temp;
	}

}


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

//make PQ and PQ_size golbal variables
event predict_collision(int i, int j) //indexes of the balls involved
{
	ball b1 = b[i];
	ball b2 = b[j];

	double t1, t2, tf;

	double del, v0, r0, r1, r2, r;

	v0 = (b2.vx - b1.vx)*(b2.vx - b1.vx) + (b2.vy - b1.vy)*(b2.vy - b1.vy);
	r0 = (b2.x - b1.x)*(b2.x - b1.x) + (b2.y - b1.y)*(b2.y - b1.y);
	del = (b2.x - b1.x)*(b2.vx - b1.vx) + (b2.y - b1.y)*(b2.vy - b1.vy);
	r1 = b1.radius;
	r2 = b2.radius;
	r = (r1+r2)*(r1+r2);

	t1 = -del + sqrt( del*del - v0*(r0 - r) );
	t1 /= v0;

	t2 = -del - sqrt( del*del - v0*(r0 - r) );
	t2 /= v0;

	if(t1<0 && t2<0)
		tf = (t1>t2)?t1:t2;
	else 
		tf = (t1<t2)?t1:t2;

	printf("Collision time: %lf\n", tf);

	if(tf>=0) //only then do we insert the event into the PQ and the LL
	{
		event collision;
		collision.t = tf;
		collision.valid = 1;
		collision.b1 = i;
		collision.b2 = j;

		PQ_insert(PQ, &PQ_size, collision);
		//list_insert(&(b[i]), &collision);		
		//list_insert(&(b[j]), &collision);

		//printf("!!!!! - %lf\n", (b[i].coll)->evt->t);

		return collision;
	}
	else
	{
		event collision;
		collision.t = tf;
		collision.valid = 0;
		collision.b1 = i;
		collision.b2 = j;

		return collision;
	}

}


void main()
{
	/*event q[10];
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
	printf("%lld\n", temp.t);

	for(i = 0; i<size; i++)
		printf("%lld\t", q[i].t);

	printf("\n");*/

	

	b[0].color = 0;
	b[0].radius = 2.0;
	b[0].x = 10; b[0].y = 0; b[0].vx = 0.0; b[0].vy = 0.0;
	b[0].coll = NULL;

	b[1].color = 0;
	b[1].radius = 1.0;
	b[1].x = 0; b[1].y = 0; b[1].vx = 2.0; b[1].vy = 0.0;
	b[1].coll = NULL;

	b[2].color = 0;
	b[2].radius = 1.0;
	b[2].x = 20; b[2].y = 0; b[2].vx = 0.5; b[2].vy = 0.0;
	b[2].coll = NULL;

	event A = predict_collision(0, 1);
	list_insert(&(b[0]), &A);
	list_insert(&(b[1]), &A);

	event B = predict_collision(1, 2);
	list_insert(&(b[1]), &B);
	list_insert(&(b[2]), &B);

	int i=0;

	for(; i<PQ_size; i++)
		printf("%lf\t", PQ[i].t);

	printf("\n");

	list *tmp = b[0].coll;
	while(tmp != NULL)
    {
    	printf("%lf\t", tmp->evt->t);
		tmp = tmp->next;
	}
	printf("\n");

	tmp = b[1].coll;
	while(tmp != NULL)
    {
    	printf("%lf\t", tmp->evt->t);
		tmp = tmp->next;
	}
	printf("\n");	

	/*event e1;
	e1.t = 3;
	e1.valid = 1;
	e1.b1 = 0;
	e1.b2 = 8;

	list_insert(&b, &e1);

	list *tmp = b.coll;
	long long int p = tmp->evt->t;

	printf("%lld\n", p);


	event e2;
	e2.t = 4;
	e2.valid = 1;
	e2.b1 = 0;
	e2.b2 = 8;

	list_insert(&b, &e2);

	while(tmp != NULL)
    {
    	printf("%lld\t", tmp->evt->t);
		tmp = tmp->next;
	}
	printf("\n");*/

}
