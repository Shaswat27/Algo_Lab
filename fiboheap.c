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
	node** A = (node **)malloc(10*sizeof(node *)); //an array of node pointers
	int i = 0,count = 0,d = 0;
	for(; i<10; i++)
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

	for(i=0; i<10; i++)
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
	//printf("Here, min = %d\n", (*min)->key);
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

		//printf("HERE, min = %d\n", (*min)->key);

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
		node *t2 = (*min2)->left;
	
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
	//remove x from the child list of y, decrement degree[y]
	if((y)->child->right == (y)->child) //y has only one child
	{
		(x)->parent = NULL;
		
		(y)->child = NULL;

		//printf("x = %d\n", (x)->key);
	}
	else if((y)->child == x)
	{
		(x)->right->left = (x)->left;
		(x)->left->right = (x)->right;

		(y)->child = (x)->right;
		(x)->parent = NULL;
	}
	else
	{
		(x)->right->left = (x)->left;
		(x)->left->right = (x)->right;

		(x)->parent = NULL;
	}

	//add x to the root list of H
	(*min)->right->left = (x);

	//printf("HERE, x = %d\n", (x)->key);

	(x)->right = (*min)->right;



	(*min)->right =(x);
	(x)->left = (*min);

	//p[x] = NIL
	(x)->parent = NULL;
	(x)->mark = 0;
}

void cascadingCut(node** min, node* y)
{
	node* z = (y)->parent;

	if(z != NULL)
	{
		if((y)->mark == 0)
			(y)->mark = 1;
		else
		{
			cut(min, y, z);
			cascadingCut(min, z);
		}
	}
}

void decreaseKey(node** min, node* x, int k)
{
	if(k > (x)->key)
	{
		printf("ERROR: new key is greater than previous key!\n");
		return;
	}

	(x)->key = k;

	node* y= (x)->parent;

	if(y != NULL && (x)->key < y->key)
	{
		cut(min, x, y);
		cascadingCut(min, y);
	}

	if((x)->key < (*min)->key)
		*min = x;
}

int main()
{
	
	/*node *min, *min2, *min3;
	int size = 0, size2 = 0, size3 = 0;
	
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
	
	//decrese key
	decreaseKey(&min3, min3->child, 0);

	printf("min: %d, min->right: %d, min->right->right: %d, min->right->right->right: %d\n", min3->key, min3->right->key, min3->right->right->key, min3->right->right->right->key);

	node* t = extractMin(&min3, &size3);

	printf("min: %d, min->child: %d, min->child->child: %d\n", min3->key, min3->child->key, min3->child->child->key); */

	int k, m, n; //k is number of lists, m is number of elements in each block, n is number of blocks in each list

	printf("Enter k: "); scanf("%d", &k);
	printf("Enter m: "); scanf("%d", &m);
	printf("Enter the number of blocks in each list: "); scanf("%d", &n);

	int** lists = (int **)malloc(k*sizeof(int *));
	int i = 0, j=0;
	for(i=0; i<k; i++)
	{
		int* list = (int *)malloc(n*m*sizeof(int));
		printf("Enter elements of list %d (sorted): ", i+1);
		for(j=0; j<n*m; j++)
		{
			int temp;
			scanf("%d", &temp);
			list[j] = temp;
		}
		lists[i] = list;
		printf("\n");
	}


	for(i=0; i<k; i++)
	{
		printf("Enter elements of list %d : ", i+1);
		for(j=0; j<n*m; j++)
		{
			printf("%d ", lists[i][j]);
		}
		printf("\n");
	}


	//start the process
	int* final_list = (int *)malloc( k*m*n*sizeof(int));
	node** mins = (node **)malloc( k*sizeof(node *) ); 
	node** temporary = (node **)malloc( k*m*n*sizeof(node *) );
	int* size = (int *)malloc(k*sizeof(int));
	int c = 0;

	for(i=0; i<n; i++)
	{
		node* last;

		for(j = 0; j<k; j++)
		{
			makeFibHeap(&mins[j], &size[j]);

			int l = 0;
			for(l = 0; l<m; l++)
			{
				node* temp = (node *)malloc(sizeof(node));

				temp->key = lists[j][i*m+l];

				fibHeapInsert(temp, &mins[j], &size[j]);

				if(l == m-1 && j==k-1)
					last = temp;
			}

			
		}

		//fibheap created for each block, now merge into one heap
		node *min, *min1 = mins[0];
		int x = 0, size_f = 0, size_1 = size[0];
		//min = fibUnion(&mins[0], &mins[1], &size[0], &size[1], &size_f);

		for(x=1; x<k; x++)
		{
			min = fibUnion(&min1, &mins[x], &size_1, &size[x], &size_f);

			size_1 = size_f;
		}

		int last_element_written;
		if(c>0)
		 	last_element_written = final_list[c-1];
		else
			last_element_written = -1000;

		if(min->key > last_element_written)
			decreaseKey(&min, last, min->key - 1);


		//now extract min
		int t = size_f;
		for(x = 0; x<t; x++)
		{
			node* temp = extractMin(&min, &size_f);
			final_list[c++] = temp->key;
		}
	}


	printf("\nFinal list: ");
	for(i=0; i<k*m*n; i++)
	{
		printf("%d ", final_list[i]);
	}
	printf("\n");


	return 0;
}