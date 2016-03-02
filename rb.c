#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

//functions for deletion
process* successor(process* n)
{
	if(n->right != NULL)
	{
		process* temp = n->right;

		while(temp->right != NULL)
			temp = temp->right;
		return temp;
	}

	process* y = n->parent;

	while(y!=NULL && n == y->right)
	{
		n = y;
		y = y->parent;
	}

	return y;
}

int is_black(process* node)
{
	//printf("Clause\n");
	if(node!=NULL)
	{
		if(node->color==0)
			return 1;
		else
			return 0;
	}
	else
		return 1;
}

void fix_delete(process** root, process* node, process* nodeParent, int nodeIsLeft) 
{
    //printf("node: %d\n", (*root)->executionTime);
    while (node != (*root) && is_black(node)) 
    {
        process* w;

        if (nodeIsLeft) 
        {
        	w = nodeParent->right;

        	//printf("Clause\n");

            if (is_black(w) != 1) 
            {
            	w->color = 0;
                nodeParent->color = 1;
                left_rotate(root, nodeParent);

                w = nodeParent->right;
            }

            if (is_black(w->left)==1 && is_black(w->right)==1) 
            {
                w->color = 1;
                node = nodeParent;
                nodeParent = node->parent;
                //printf("Clause2\n");
                if(nodeParent!=NULL)
                	nodeIsLeft = (node == nodeParent->left)?1:0;
                else
                	nodeIsLeft = 0;
            } 
            else 
            {
                
                if (is_black(w->right) == 1) 
                {
                    w->left->color = 0;
                    w->color = 1;
                    right_rotate(root, w);
                    w = nodeParent->right;
                }

                w->color = nodeParent->color;
                nodeParent->color = 0;
                if (w->right != NULL) 
                {
                    w->right->color = 0;
                }
                left_rotate(root, nodeParent);
                node = *root;
                nodeParent = NULL;
            }
        } 
        else 
        {   
            w = nodeParent->left;

            if (is_black(w) != 1) 
            {
                w->color = 0;
                nodeParent->color = 1;
                right_rotate(root, nodeParent);
                w = nodeParent->left;
            }

            if (is_black(w->left)==1 && is_black(w->right)==1) 
            {
                w->color = 1;
                node = nodeParent;
                nodeParent = node->parent;
                if(nodeParent!=NULL)
                    nodeIsLeft = (node == nodeParent->left)?1:0;
                else
                    nodeIsLeft = 0;
            } 
            else 
            {
                if (is_black(w->left) == 1) 
                {
                    w->right->color = 0;
                    w->color = 1;
                    left_rotate(root, w);
                    w = nodeParent->left;
                }

                w->color = nodeParent->color;
                nodeParent->color = 0;                
                if (w->left != NULL) 
                {
                    w->left->color = 1;
                }                
                right_rotate(root, nodeParent);
                node = *root;
                nodeParent = NULL;
            }
        }
    }

    //printf("\n\nChange Node Color: %d\n\n", node->executionTime);

    node->color = 0;
}

process* delete(process** root, process* node) 
{
    printf("Node to be deleted: %d\n", node->executionTime);
    process* y;
    if (node->left == NULL || node->right == NULL) 
    {
        y = node;
    } 
    else 
    {
        y = successor(node);
    }

    process* x;
    if (y->left != NULL) 
    {
        x = y->left;
    } 
    else 
    {
        x = y->right;
    }
    if (x != NULL) 
    {
        x->parent = y->parent;
    }

    process* xParent = y->parent;

    int yIsLeft = 0;
    if (y->parent == NULL) 
    {
        *root = x;
    } 
    else if (y == y->parent->left) 
    {
        y->parent->left = x;
        yIsLeft = 1;
    } 
    else 
    {
        y->parent->right = x;
        yIsLeft = 0;
    }

    if (y != node) 
    {
        node->executionTime = y->executionTime;
        node->processId = y->processId;
        node->priority = y->priority;
        node->color = y->color;
    }

    if (y->color == 0 && (*root)!=NULL) 
    {
        //if(xParent==NULL)
        	//printf("---\n");
        //if(xParent != NULL)
        //printf("before fix_delete\n");
        fix_delete(root, x, xParent, yIsLeft);
        //printf("after fix_delete\n");
    }

    return y;
}

//function to find minimum
process* min(process* root)
{
	process* curr = root;
	while(curr->left != NULL)
		curr=curr->left;
	//printf("min = %d\n", curr->executionTime);
	return curr;
}

//create process
process* create_process(int c)
{
	process* temp = (process *)malloc(sizeof(process));

	temp->executionTime = 1 + (rand())%1000;
	temp->color = 1;
	temp->priority = 1 + (rand())%4;
	temp->processId = c;

	temp->parent = NULL;
	temp->left =  NULL;
	temp->right = NULL;

	return temp;
}

//THE LOOP!
void scheduler(int N, int x, int M, int t, int c, process** root)
{
	printf("x = %d, c = %d\n", x, c);

	if(x<N && c<M) //if number of live processes<=current number of live processes and c<total number of processes created
	{
		process* new_process = create_process(c++);

		if( (*root) == NULL )
		{
			printf("first node: %d\n", new_process->executionTime);
			new_process->color = 0;
			*root = new_process;
			x++;
		}	
		else
		{
			printf("node insert: %d\n", new_process->executionTime);
			insert(root, new_process);

			//if(c==3)
        	//	printf("root = %d, root->left = %d, root->right = %d\n", (*root)->color, (*root)->left->color,(*root)->right->color);

			x++;
		}	
		//if(c==5)
		//printf("\nRoot: %d, %d, %d, %d\n\n", (*root)->color, (*root)->left->color, (*root)->right->color, (*root)->left->right->color);
	}

	if(x>0)
	{
		
		process* execute = min(*root);
		
		//printf("before delete\n");

		/*if(c==1)
			printf("root = %d, color = %d\n", (*root)->executionTime, (*root)->color);

		if(c==2)
			printf("root = %d, color = %d\n, root->right = %d\n, color = %d", (*root)->executionTime, (*root)->color, (*root)->right->executionTime, (*root)->right->color);
		*/
		//if(*root != NULL && (*root)->left!=NULL && (*root)->right!=NULL)
		//printf("\n\nroot = %d, root->left = %d, root->right = %d\n", (*root)->executionTime, (*root)->left->executionTime,(*root)->right->executionTime);

		execute = delete(root, execute);

		//printf("after delete\n");

		execute->executionTime -= t*(execute->priority);

		if(execute->executionTime > 0) //still has to be inserted
		{
			execute->color = 1; //color should be red for any insertion process
			insert(root, execute);
		}	
		else
			x--;

		scheduler(N, x, M, t, c, root);
	}

}


void main()
{
	int N = 10, x = 0, M = 10; //max number of live processes, current number of processes, number of processes to be completed
	int t = 50; //time for each scheduling = priority*t;
	int c = 0; //total number of processes created

	//srand(time(NULL));

	process* processTree = NULL;

	scheduler(N, x, M, t, c, &processTree);

	/*process* n1 = (process *)malloc(sizeof(process));
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

	printf("%d %d %d %d\n", processTree->executionTime, processTree->left->executionTime, processTree->right->executionTime, processTree->left->left->executionTime);

	
	delete(&processTree, n3);

	printf("%d %d %d\n", processTree->color, processTree->left->color, processTree->right->color);		

	process* search = lookup(processTree, 9);

	printf("%d\n", search->executionTime);*/
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
