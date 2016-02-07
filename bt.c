#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct n
{
	int key;
	struct n* parent;
	struct n* l_child;
	struct n* r_child;
} node;

int* check; //1 if used, 0 if unused
int size;

node* tree(int n, node* start)
{
	node* temp = (node*)malloc(sizeof(node));

	if(start == NULL)
		size = n;

	if(n==0) //stopping condition
	{
		temp = NULL;
		return temp;
	}

	do
	{
		int sign = rand()%2;
		if(sign)
			temp->key = rand()%size;
		else
			temp->key = -rand()%size;
	}while(check[temp->key]==1);

	check[temp->key] = 1;
	
	temp->parent = start;


	int x = 40 + rand()%21; 
	float p = x/100.0;

	int m = (int)(p*n);
	int l = n-m-1;

	temp->l_child = NULL;
	temp->r_child = NULL;

	//start = temp;
	temp->l_child = tree(m, temp);
	temp->r_child = tree(l, temp);


	/*printf("\nn = %d, temp.key = %d", n, temp->key);
	if(temp->l_child!=NULL && temp->r_child!=NULL)
		printf("\nstart = %d, l_child = %d, r_child = %d", temp->key, temp->l_child->key, temp->r_child->key);
	else if(temp->l_child==NULL && temp->r_child!=NULL)
		printf("\nstart = %d, l_child = NULL, r_child = %d", temp->key, temp->r_child->key);
	else if(temp->l_child!=NULL && temp->r_child==NULL) 
		printf("\nstart = %d, l_child = %d, r_child = NULL", temp->key, temp->l_child->key);
	else 
		printf("\nstart = %d, l_child = NULL, r_child = NULL", temp->key);*/

	return temp;
}

void inorder(node* p)
{
	if(p==NULL)
		return;

	inorder(p->l_child);
	
	printf("%d ",p->key);
	
	inorder(p->r_child);
	
}

void preorder(node* p)
{
	if(p==NULL)
		return;

	printf("%d ",p->key);

	preorder(p->l_child);
	
	preorder(p->r_child);
}


void postorder(node* p)
{
	if(p==NULL)
		return;

	postorder(p->l_child);

	postorder(p->r_child);

	printf("%d ",p->key);
}

int main()
{
	//srand(time(NULL));
	int n;

	printf("\nEnter the number of nodes: ");
	scanf("%d", &n);

	check = (int *)malloc((2*n-1)*sizeof(int));
	int i = 0; 
	for(i=0; i<n; i++)
		check[i] = 0;
	node* t = NULL;
	t = tree(n, t); //create the tree

	printf("\nInorder traversal: \n");
	inorder(t);

	printf("\nPreorder traversal: \n");
	preorder(t);

	printf("\nPostorder traversal: \n");
	postorder(t);

	printf("\n");
	return 0;	
}