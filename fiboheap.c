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

node *min, *min2, *min3;
int size = 0, size2 = 0, size3 = 0;

void makeFibHeap(node** min, int *size)
{
	*size = 0;
	*min = NULL;
}

void fibHeapInsert(node* x, node** min, int *size)
{
	x->degree = 0;
	x->parent = NULL;
	x->child = NULL;
	x->left = x;
	x->right = x;
	x->mark = 0;

	//concatenate the root list containing x with root list H
	if( *min != NULL)
	{
		x->right = *min;
		x->left = (*min)->left;
		(*min)->left = x;
		x->left->right = x;
		if(x->key < (*min)->key)
			*min = x; 
	}
	else
	{
		*min = x;
		(*min)->left = *min;
		(*min)->right = *min;
	}

	*size += 1;
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

void consolidate(node** min, int *size)
{
	//create auxillary array A
	int D = (int)( ceil(log2(*size)) );
	node** A = (node **)malloc(D*sizeof(node *)); //an array of node pointers
	int i = 0,count = 0,d = 0;
	for(; i<D; i++)
		A[i] = NULL;

	node* w = *min;
	node* x;
	node* y;

	do
	{
		count++;
		w = w->right;
	}while(w != (*min));

	w = *min;

	for(i = 0; i<count; i++)
	{
		x = w;
		w = w->right;
		d = x->degree;
		while(A[d] != NULL)
		{
			y = A[d];
			if(x->key > y->key)
			{
				node *temp = y;
				y = x;
				x = temp;
			}
			heapLink(&y, &x);
			A[d] = NULL;
			d++;
		}
		A[d] = x;
	}

	*min = NULL;

	for(i=0; i<D; i++)
	{
		if(A[i] != NULL)
		{
			if( *min == NULL)
			{
				*min = A[i];
				(*min)->right = *min;
				(*min)->left = *min;
				(*min)->parent = NULL;
			}
			else
			{
				node *p;
				p = (*min)->right;
				(*min)->right = A[i];
				A[i]->left = (*min);
				A[i]->right = p;
				p->left = A[i];
				if(A[i]->key < (*min)->key)
					*min = A[i];
			}
		}
	}
	return;
}

node* extractMin(node** min, int *size)
{
	node* z = *min;
	node* x = z->child;
	node* a = z->right;
	node* b = z->right->right;
	node* y;

	if(z != NULL)
	{
		
		if(x != NULL)
		{
			y = x->left;
			a->right = x;
			x->left = a;
			y->right = b;
			b->left = y;
			while(x != b)
			{
				x->parent = NULL;
				x = x->right;
			}
		}



		//remove z from root list
		a = z->left; b = z->right;
		a->right = b; b->left = a;

		if(z == z->right)
		{
			*min = NULL;
		}
		else
		{
			*min = z->right;
			consolidate(min, size);
		}	

		*size -= 1;
	}
	return z;
}

node* fibUnion(node** min1, node** min2, int *size1, int *size2, int *size)
{
	node* min;
	makeFibHeap(&min, size);

	min = *min1;

	//concatenate root list of H with H2
	if( min == NULL && *min2 == NULL)
	{
		*size = 0;
		return(NULL);
	}
	else if( min != NULL && *min2 == NULL)
	{
		*size = *size1;
		return(min);
	}
	else if( min == NULL && *min2 != NULL)
	{
		*size = *size2;
		return(*min2);
	}
	else
	{
		node *t1 = min->left;
		node *t2 = (*min2)->right;
	
		t1->right = (*min2);
		(*min2)->left = t1;
	
		min->left = t2;
		t2->right = min;
	}

	if((*min2)->key < min->key)
		min = *min2;

	*size = (*size1) + (*size2);

	return min;	
}

void cut(node** min, node* x, node* y)
{

}


int main()
{
	makeFibHeap(&min, &size);

	int i = 100;
	for(; i>=1; i--)
	{
		node *x = (node *)malloc(sizeof(node));
		x->key = i;
		fibHeapInsert(x, &min, &size);
	}

	int temp = size;

	for(i = 0; i<temp; i++)
	{
		node* y = extractMin(&min, &size);

		printf("Min: %d\n", y->key);
	}

	printf("\n");

	/*
	//FIRST HEAP
	makeFibHeap(&min, &size);

	node *x = (node *)malloc(sizeof(node));
	x->key = 1;

	fibHeapInsert(x, &min, &size);

	node *y = (node *)malloc(sizeof(node));
	y->key = 3;

	fibHeapInsert(y, &min, &size);

	node *z = (node *)malloc(sizeof(node));
	z->key = 0;

	fibHeapInsert(z, &min, &size);

	display(min);

	printf("min -> %d\n", min->key);

	z = extractMin(&min, &size);
	printf("Extracted min -> %d\n", z->key);

	//display(min);

	printf("min: %d, Child: %d, Child->left: %d, Child->right: %d\n", min->key, x->child->key, x->child->left->key, x->child->right->key);


	//SECOND HEAP
	makeFibHeap(&min2, &size2);

	node *x2 = (node *)malloc(sizeof(node));
	x2->key = 10;

	fibHeapInsert(x2, &min2, &size2);

	node *y2 = (node *)malloc(sizeof(node));
	y2->key = 30;

	fibHeapInsert(y2, &min2, &size2);

	//UNION
	min3 = fibUnion(&min, &min2, &size, &size2, &size3);

	printf("min: %d, min->right: %d, min->right->right: %d, min->child: %d, min->child->left: %d, min->child->right: %d\n", min3->key, min3->right->key, min3->right->right->key, min3->child->key, min3->child->left->key, min3->child->right->key);
	*/
	return 0;
}