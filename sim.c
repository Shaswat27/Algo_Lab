#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define T 30

//assuming square box
#define X 30
#define Y 30

double time_now = 0;

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

event *PQ[100], aux[100]; //auxillary array
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

void swap(event *Q[], int i, int j)
{
	event *temp = Q[j];
	Q[j] = Q[i];
	Q[i] = temp;
}

void heapify(event *Q[], int heap_size, int i)
{
	int l = left(i);
	int r = right(i);
	
	int minimum = i;
	
	if(l<heap_size)
	{
		if((*Q[l]).t<(*Q[i]).t)
			minimum = l;
		else minimum = i;
	}	
	
	if(r<heap_size)
	{
		if((*Q[r]).t<(*Q[minimum]).t)
			minimum = r;
	}
	
	
	if( minimum != i)
	{
		swap(Q, i, minimum);
		heapify(Q, heap_size, minimum);
	}
}

int heap_insert(event *Q[], int *heap_size, event *k)
{
	*heap_size += 1;
	
	int i = (*(heap_size)) - 1;

	//printf("\nTo be inserted: %lf, b1: %d, b2: %d\n", (*k).t, (*k).b1, (*k).b2);

	while(i>0 && (*PQ[parent(i)]).t > (*k).t)
	{
		PQ[parent(i)] = PQ[i];//Q[i] = Q[parent(i)];
		i = parent(i);
	}
	
	PQ[i] = k;
	//printf("PQ[%d] = %lf\n", i, (*PQ[i]).t);
	return i;
}

event extract_min(event *Q[], int *heap_size)
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
	event min = *Q[0];
	Q[0] = Q[(*heap_size) - 1];
	*heap_size = *heap_size - 1;
	heapify(Q, *heap_size, 0);
	return min;
}

int PQ_insert(event *Q[], int* q_size, event *k)
{
	return heap_insert(Q, q_size, k);
}

event PQ_extract(event *Q[], int* q_size)
{
	return extract_min(Q, q_size);
}

event* predict_collision(int i, int j, int *p) //indexes of the balls involved
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

	tf += time_now;

	//printf("Collision time: %lf\n", tf);

	if(tf>=time_now) //only then do we insert the event into the PQ and the LL
	{
		event *collision = (event *)malloc(sizeof(event));
		collision->t = tf;
		collision->valid = 1;
		collision->b1 = i;
		collision->b2 = j;

		*p = PQ_insert(PQ, &PQ_size, collision);
		//list_insert(&(b[i]), &collision);		
		//list_insert(&(b[j]), &collision);

		//printf("!!!!! - %lf\n", (b[i].coll)->evt->t);

		return collision;
	}
	else
	{
		event *collision = (event *)malloc(sizeof(event));;
		collision->t = tf;
		collision->valid = 0;
		collision->b1 = i;
		collision->b2 = j;

		return collision;
	}

}

void update_state(event current)
{
	double delta_t = current.t - time_now;

	int i = 0;

	// update the state of the two balls
	for(; i<5; i++)
	{
		b[i].x = b[i].x + (b[i].vx)*(delta_t);
		b[i].y = b[i].y + (b[i].vy)*(delta_t);
	}

	// fast forward to event time
	time_now = current.t;
}

void update_velocity(event current)
{
	int u = current.b1, v = current.b2;

	//collision is occuring at this time
	double A = atan2(b[u].y-b[v].y, b[u].x-b[v].x);
	double D1 = atan2(b[u].vy, b[u].vx);
	double D2 = atan2(b[v].vy, b[v].vx);

	double u1 = sqrt( b[u].vx*b[u].vx + b[u].vy*b[u].vy );
	double u2 = sqrt( b[v].vx*b[v].vx + b[v].vy*b[v].vy );

	double _t = u2*cos(D2-A);
	double _s = u1*cos(D1-A);

	b[u].vx = _t*cos(A) - u1*sin(D1-A)*sin(A);
	b[u].vy = _t*sin(A) + u1*sin(D1-A)*cos(A);

	b[v].vx = _s*cos(A) - u2*sin(D2-A)*sin(A);
	b[v].vy = _s*sin(A) + u2*sin(D2-A)*cos(A);
		
	printf("Update velocities of %d and %d\n to (%lf, %lf) and (%lf, %lf)", u, v, b[u].vx, b[u].vy, b[v].vx, b[v].vy);

	// now traverse the list and mark the events as invalid
	list **head = &(b[u].coll);
	while( *head != NULL)
	{
		// invalidate the events
		//printf("Invalidate %f ", (*head)->evt->t );
		(*head)->evt->valid = 0;

		//printf("%p %p\n\n", &( (*head)->evt->valid ), &((*PQ[0]).valid));

		// move forward
		*head = (*head)->next;
	}
	// deallocate the list
	(*head) = NULL;

	// now traverse the list and mark the events as invalid
	list **head2 = &(b[v].coll);
	while( *head2 != NULL)
	{
		// invalidate the events
		// printf("Invalidate %f ", (*head2)->evt->t );
		(*head2)->evt->valid = 0;

		// printf("%p %p\n", &( (*head2)->evt->valid ), &((*PQ[0]).valid));

		// move forward
		*head2 = (*head2)->next;
	}
	// deallocate the list
	(*head2) = NULL;

}

event* predict_wall_collision(int i)
{
	ball b1 = b[i];

	double x1, y1, x2, y2;
	double x, y;
	double tf, t1, t2;

	//in X direction
	if(abs(b1.vy)<=abs(b1.vx)) //slope less than 45deg
	{
		if(b1.vx != 0)
		{
			y1 = (b1.vy/b1.vx)*(b1.radius) + b1.y - (b1.vy/b1.vx)*b1.x;
			y2 = (b1.vy/b1.vx)*(X-b1.radius) + b1.y - (b1.vy/b1.vx)*b1.x;
		}
		else
		{
			y1 = b1.y;
			y2 = y1;	
		}

		if(b1.vy>0)
		{
			y = (y1>y2)?y1:y2;
			x = 0.0 + b1.radius;
		}
		else
		{
			y = (y1>y2)?y2:y1;
			x = X - b1.radius;
		}

		//printf("(%lf, %lf)\n", x, y);
	}

	//in Y direction
	else
	{
		if(b1.vy != 0)
		{
			x1 = (b1.vx/b1.vy)*(b1.radius) + b1.x - (b1.vx/b1.vy)*b1.y;
			x2 = (b1.vx/b1.vy)*(Y-b1.radius) + b1.x - (b1.vx/b1.vy)*b1.y;
		}
		else
		{
			x1 = b1.x;
			x2 = y1;	
		}

		if(b1.vx>0)
		{
			x = (x1>x2)?x1:x2;
			y = 0.0 + b1.radius;
		}
		else
		{
			x = (x1>x2)?x2:x1;
			y = Y - b1.radius;
		}
	}

	// check time to reach (x, y)
	double v = sqrt(b1.vx*b1.vx + b1.vy*b1.vy);
	double dist = sqrt( (b1.x - x)*(b1.x - x) + (b1.y - y)*(b1.y - y) );

	tf = time_now + dist/v;

	event *collision = (event *)malloc(sizeof(event));
	collision->t = tf;
	collision->valid = 1;
	collision->b1 = i;
	collision->b2 = i;

	return collision;
}

void update_velocity_wall(event current)
{
	int u = current.b1;

	if(b[u].x == X-b[u]-radius || b[u].x == b[u].radius)
	{
		b[u].vx = -b[u].vx;
	}
	else if(b[u].x == X-b[u]-radius || b[u].x == b[u].radius)
	{
		b[u].vy = -b[u].vy;
	}

	list **head = &(b[u].coll);
	while( *head != NULL)
	{
		// invalidate the events
		(*head)->evt->valid = 0;

		*head = (*head)->next;
	}
	// deallocate the list
	(*head) = NULL;
}


void main()
{
	b[0].color = 0;
	b[0].radius = 2.0;
	b[0].x = 12; b[0].y = 3; b[0].vx = 0.0; b[0].vy = 0.0;
	b[0].coll = NULL;

	b[1].color = 0;
	b[1].radius = 1.0;
	b[1].x = 2; b[1].y = 3; b[1].vx = 2.0; b[1].vy = 0.0;
	b[1].coll = NULL;

	b[2].color = 0;
	b[2].radius = 1.0;
	b[2].x = 22; b[2].y = 3; b[2].vx = 0; b[2].vy = 0.0;
	b[2].coll = NULL;

	int i = 0, j = 0, k=0, pos = 0, itr = 0;

	/*do // while in defined time window
	{
		//predict events
		for(i=0; i<3; i++)
		{
			predict_wall_collision(i);
			for(j=i+1; j<3; j++)
			{
				event *ret = predict_collision(i, j, &pos);
				if(ret->valid == 1) //if the event is valid
				{
					printf("Insert event: %lf in %d, and %d\n", ret->t, i, j);
					list_insert(&(b[i]), ret);
					list_insert(&(b[j]), ret);
		
				}
				//k++;
			}
		}

		int h = 0;
		printf("Priority Queue: ");
		for(; h<PQ_size; h++)
			printf("%lf,%d,%d\t", (*PQ[h]).t, (*PQ[h]).b1, (*PQ[h]).b2);
		printf("\n");

		event current = PQ_extract(PQ, &PQ_size);

		if(current.valid == 1) //if the event is valid
		{
			printf("Valid event: %lf\n", current.t);

			//fast forward all the balls
			update_state(current);

			//update the velocities of involved balls and deallocate their lists after marking them as invalid
			update_velocity(current);

			//printf("%lf\n", time_now);
		}
		itr++;
		printf("\n");
		
	}while(itr < 10);*/

	int l = 0;
	for(; l<3; l++)
	{
		event *cs = predict_wall_collision(l);
		printf("%d - %lf\n", l, cs->t);
	}

	
}
