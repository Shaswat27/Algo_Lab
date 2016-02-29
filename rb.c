#include <stdio.h>
#include <stdlib.h>

typedef struct n
{
	int processId;
	int executionTime;
	int priority;
	int color; //0 for black, 1 for red

	//parent pointer
	struct n* parent;
	//child pointers
	struct n* left;
	struct n* right;
} process;

int N = 10, x = 0, M = 10; //max number of live processes, current number of processes, number of processes to be completed
int t = 50; //time for each scheduling = priority*t;

//grandparent
process* grandparent(process* n)
{
	if(n->parent != NULL && n->parent->parent !=NULL)
		return n->parent->parent;
	else 
		return NULL;
}

//uncle
process* uncle(process* n)
{
	process *g = grandparent(n);
	if(g != NULL)
	{
		if(n->parent == g->left)
			return g->right;
		else
			return g->left;
	}
	else
		return NULL;
}

//function for rotation
//right rotate
void right_rotate(process** root, process* n)
{
	process *y = n->left;
	n->left = y->right;

	if(y->right != NULL)
		y->right->parent = n;
	y->parent = n->parent;

	if(n->parent == NULL)
		*root = y;
	else if(n = n->parent->right)
		n->parent->right = y;
	else
		n->parent->left = y;
	y->right = n;
	n->parent = y;
}

void left_rotate(process** root, process* n)
{
	process *y = n->right;
	n->right = y->left;

	if(y->left != NULL)
		y->left->parent = n;
	y->parent = n->parent;

	if(n->parent == NULL)
		*root = y;
	else if(n = n->parent->left)
		n->parent->left = y;
	else
		n->parent->right = y;
	y->left = n;
	n->parent = y;
}

//functions for insertion
void bst_insert(process** root, process* n)
{
	if((*root) == NULL)
	{
		n->parent = NULL; 
		n->left = NULL;
		n->right = NULL;
		*root = n;
	}
	else
	{
		if(n->executionTime < (*root)->executionTime)
		{
			if((*root)->left != NULL)
				bst_insert(&((*root)->left), n);
			else
			{
				(*root)->left = n;
				n->parent = *root;
				n->left = NULL;
				n->right = NULL;
			}
		}
		else
		{
			if((*root)->right != NULL)
				bst_insert(&((*root)->right), n);
			else
			{
				(*root)->right = n;
				n->parent = (*root);
				n->left = NULL;
				n->right = NULL;
			}
		}
	}	
}

void insert1(process** root, process* n);
void insert2(process** root, process* n);
void insert3(process** root, process* n);
void insert4(process** root, process* n);
void insert5(process** root, process* n);

void insert(process** root, process* node)
{
	bst_insert(root, node);

	//printf("%d\n", node->parent->right->executionTime);

	insert1(root, node);
}

void main()
{
	process* n1 = (process *)malloc(sizeof(process));
	n1->processId = 1;
	n1->executionTime = 10;
	n1->color = 0;
	n1->parent = NULL;
	n1->left = NULL;
	n1->right = NULL;

	process* processTree = NULL; //root of the process tree

	insert(&processTree, n1);

	process* n2 = (process *)malloc(sizeof(process));
	n2->processId = 1;
	n2->executionTime = 9;
	n2->color = 1;
	n2->parent = NULL;
	n2->left = NULL;
	n2->right = NULL;

	insert(&processTree, n2);

	//printf("%d\n%d\n", n1->executionTime, n1->->executionTime);

	process* n3 = (process *)malloc(sizeof(process));
	n3->processId = 1;
	n3->executionTime = 8;
	n3->color = 1;
	n3->parent = NULL;
	n3->left = NULL;
	n3->right = NULL;

	insert(&processTree, n3);

	printf("%d %d %d\n", processTree->executionTime, processTree->left->executionTime, processTree->right->executionTime);

	process* n4 = (process *)malloc(sizeof(process));
	n4->processId = 1;
	n4->executionTime = 7;
	n4->color = 1;
	n4->parent = NULL;
	n4->left = NULL;
	n4->right = NULL;

	insert(&processTree, n4);

	printf("%d %d %d %d\n", processTree->executionTime, processTree->left->executionTime, processTree->right->executionTime, processTree->left->left->color);
	
}

void insert5(process** root, process* n)
{
	process *g = grandparent(n);

	g->color = 1; //paint red
	n->parent->color = 0; //paint black
	if(n == n->parent->left)
		right_rotate(root, g);
	else
	{
		//printf("CASE 5\n");
		left_rotate(root, g);
	}
		
}

void insert4(process** root, process* n)
{
	process *g = grandparent(n);
	process *p = n->parent;

	if(n == n->parent->right && n->parent == g->left)
	{
		left_rotate(root, n->parent);
		insert5(root, n->left);
	}	
	else if(n == n->parent->left && n->parent == g->right)
	{
		right_rotate(root, n->parent);
		insert5(root, n->right);
	}
	else
		insert5(root, n);	
}

void insert3(process** root, process* n)
{
	process *g = grandparent(n);
	process *u = uncle(n);

	if(u != NULL)
	{
		if(u->color == 1)
		{
			u->color = 0;
			n->parent->color = 0;
			g->color = 1;
			insert1(root, g);
		}
	}
	else
		insert4(root, n);
}

void insert2(process** root, process* n)
{
	if(n->parent->color == 0)
		return; 
	else
		insert3(root, n);
}

void insert1(process** root, process* n)
{
	if(n->parent == NULL)
		n->color = 0;
	else
		insert2(root, n);
}
