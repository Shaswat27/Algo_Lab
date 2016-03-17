#include <stdio.h>
#include <stdlib.h>

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

	return 0;
}