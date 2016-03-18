#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct n
{
	int key;
    int degree;
    struct n* parent;
    struct n* child;
    struct n* left;
    struct n* right;
    int mark; //0 as false, 1 as true
} node;

node* H; //the fiboheap object
node* min;
int size = 0;

void makeFibHeap()
{
	H = NULL;
	size = 0;
	min = NULL;
}

void fibHeapInsert(node* x)
{
	x->degree = 0;
	x->parent = NULL;
	x->child = NULL;
	x->left = x;
	x->right = x;
	x->mark = 0;

	//concatenate the root list containing x with root list H
	if( min != NULL)
	{
		x->right = min;
		x->left = (min)->left;
		(min)->left = x;
		x->left->right = x;
		if(x->key < (min)->key)
			min = x; 
	}
	else
	{
		min = x;
		(min)->left = min;
		(min)->right = min;
	}

	H = min;

	size += 1;
}

void display(node *min1)
{
	node *q,*chil;
	if(min == NULL)
	{
		printf("Fibonacci heap is empty\n");
		return;
	}
	
	q = min;
	
	printf("Fibonacci heap display: ");
	do
	{
		printf("%d ",q->key);
		if(q->child != NULL)
		{
			display(q->child);
		}
		q=q->right;
	}while(q != (min));
	printf("\n");
}


void heapLink(node** y, node** x)
{
	//remove y from root list
	(*y)->left->right = (*y)->right;
	(*y)->right->left = (*y)->left;

	//make y a child of x
	(*x)->degree += 1;
	if((*x)->child != NULL)
	{
		(*y)->parent = *x;
		(*y)->right = (*x)->child;
		(*y)->left = (*x)->child->left;
		(*x)->child->left = *y;
		(*y)->left->right = *y;
	}
	else
	{
		(*x)->child = *y;
		(*y)->parent = *x;

		(*y)->right = (*y);
		(*y)->left = (*y);
	}


	(*y)->mark = 0;
}

void consolidate()
{
	//create auxillary array A
	int D = (int)( ceil(log2(size)) );
	node** A = (node **)malloc(D*sizeof(node *)); //an array of node pointers
	int i = 0;
	for(; i<D; i++)
		A[i] = NULL;

	node* w = min;

	//for each node in root list of H
	do
	{
		node* x = w;
		int d = x->degree;

		while(A[d] != NULL)
		{
			node *y = A[d]; //another node with same degree as x
			if(x->key > y->key)
			{
				//exchange x & y
				node* temp = y;
				y = x;
				x = temp;
			}
			heapLink(&y,&x);
			A[d] = NULL;
			d += 1;
		}
		A[d] = x;

		//printf("x->right = %p, w = %d, min = %p\n", x->right, w->key, min);
		w = x;
		w = w->right;
	}while(w != min);

	node* temp = min;
	min = NULL;
	for(i=0; i<D; i++)
	{
		if(A[i] != NULL)
		{
			if(temp != NULL)
			{
				(A[i])->right = temp;
				(A[i])->left = (temp)->left;
				(temp)->left = A[i];
				(A[i])->left->right = A[i];
			}
			else
			{
				temp = A[i];
				(temp)->left = temp;
				(temp)->right = temp;
			}

			if(min == NULL || (A[i])->key < min->key)
				min = A[i];
		}
	}
}

node* extractMin()
{
	node* z = min;
	if(z != NULL)
	{
		if(z->child != NULL)
		{
			do//for each child of z
			{
				node **x;
				*x = z->child;

				(*x)->right = min->right;
				(*x)->left = min->left;
				(*x)->left->right = *x;
				(*x)->right->left = *x;
				
				z = z->right;
			}while(z != min);
		}

		//remove z from root list
		z->left->right = z->right;
		z->right->left = z->left;


		if(z == z->right)
		{
			min = NULL;
		}
		else
		{
			min = z->right;
			consolidate();
		}	

		size -= 1;
	}
	return z;
}



int main()
{
	makeFibHeap();

	node *x = (node *)malloc(sizeof(node));
	x->key = 1;

	fibHeapInsert(x);

	node *y = (node *)malloc(sizeof(node));
	y->key = 3;

	fibHeapInsert(y);

	node *z = (node *)malloc(sizeof(node));
	z->key = 0;

	fibHeapInsert(z);

	display(min);

	printf("min -> %d\n", min->key);

	z = extractMin();
	printf("Extracted min -> %d\n", z->key);

	//display(min);

	printf("min: %d, Child: %d, Child->left: %d, Child->right: %d\n", min->key, x->child->key, x->child->left->key, x->child->right->key);

	return 0;
}