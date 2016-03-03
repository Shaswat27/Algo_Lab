#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long long int time_stamp = 0;
FILE *record;

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

	//linked list structure for same value
	struct n* next;

	//time_stamps
	unsigned long long int creation_time;
	unsigned long long int completion_time;

	int i;
	unsigned long long int exec_times[1000];

	int j;
	unsigned long long int preempt_times[1000];
} process;

//to print process data
void print_data(process* node)
{
	fprintf(record, "\n%d\t\t\t", node->processId);
	fprintf(record, "%d\t\t\t\t\t", node->priority);
	fprintf(record, "%llu\t\t\t\t", node->creation_time);
	
	int x=0;
	for(;x<node->i;x++)
		fprintf(record, "%llu ", node->exec_times[x]);
	int l = 12 - x;
	for(;l>=0;l--)
		fprintf(record, "\t");

	x=0;
	for(;x<node->j;x++)
		fprintf(record, "%llu ", node->preempt_times[x]);
	l = 12 - x;
	for(;l>=0;l--)
		fprintf(record, "\t");

	fprintf(record, "%llu", node->completion_time);

	/*printf("\n\nProcess ID: %d\n", node->processId);
	printf("Proccess Priority: %d\n", node->priority);
	printf("Process Creation Time: %llu\n", node->creation_time);
	
	printf("Process Execution Times: ");
	int x=0;
	for(;x<node->i;x++)
		printf("%llu ", node->exec_times[x]);
	printf("\n");

	printf("Process Preempt Times: ");
	x=0;
	for(;x<node->j;x++)
		printf("%llu ", node->preempt_times[x]);
	printf("\n");

	printf("Process Completion Time: %llu\n\n", node->completion_time);*/
}

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
	else if(n == n->parent->right)
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

//linked list insertion
void list_insert(process** root, process* n)
{
	process* current;

    //head insertion
    if ((*root)->next==NULL)
    {
        if((*root)->priority <= n->priority)
        {
        	n->next = (*root)->next;
        	(*root)->next = n;
        }
        else
        {
        	n->next = (*root)->next;
        	(*root)->next = n;
        	
        	//now swap data
        	int temp = 0;
        	
        	temp = n->processId;
        	n->processId = (*root)->processId;
        	(*root)->processId = temp;

        	temp = n->priority;
        	n->priority = (*root)->priority;
        	(*root)->priority = temp;
        }
    }
    else
    {
        current = (*root);
        while (current->next !=NULL && current->next->priority < n->priority)
        {
            current = current->next;
        }
        n->next = current->next;
        current->next = n;
    }
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
		else if(n->executionTime > (*root)->executionTime)
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
		else //if the execution times are equal
		{
			list_insert(root, n);
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
    while (node != (*root) && is_black(node)) 
    {
        process* w;

        if (nodeIsLeft) 
        {
        	w = nodeParent->right;

        	if (is_black(w) != 1) 
            {
            	w->color = 0;
                nodeParent->color = 1;
                left_rotate(root, nodeParent);

                w = nodeParent->right;
            }

            if(w!=NULL)
            {
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
            else  //if w is NULL
            {
            	node = nodeParent;
                nodeParent = node->parent;
                if(nodeParent!=NULL)
                	nodeIsLeft = (node == nodeParent->left)?1:0;
                else
                	nodeIsLeft = 0;
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
	temp->next = NULL;

	temp->creation_time = time_stamp;
	temp->completion_time = 0;
	temp->i = 0;
	temp->j = 0;

	return temp;
}

//THE LOOP!
void scheduler(int N, int x, int M, int t, int c, process** root)
{
	time_stamp++;
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

			x++;
		}	
	}

	if(x>0)
	{
		
		process* execute = min(*root);

		if(execute->next != NULL) //we have linked list elements
		{
			process* pop_prev = execute;
			process* pop = execute->next;
			while(pop->next != NULL)
			{
				pop = pop->next;
				pop_prev = pop_prev->next;
			}

			//now, pop has the last element in the linked list
			pop->exec_times[(pop->i)++] = time_stamp;
			int temp = pop->executionTime;
			pop->executionTime -= t*(pop->priority);
		
			if(pop->executionTime<0)
				time_stamp += temp;
			else
				time_stamp += t*(pop->priority);

			pop->preempt_times[(pop->j)++] = time_stamp; 

			if(pop->executionTime > 0) //still has to be inserted
			{
				pop->color = 1; //color should be red for any insertion process
				insert(root, pop);
			}
			else
			{
				x--;
				pop->completion_time = time_stamp;
				print_data(pop);
			}
	
			pop_prev->next = pop->next; //since this was the last node

			scheduler(N, x, M, t, c, root);
		}

		else
		{
			execute = delete(root, execute);

			execute->exec_times[(execute->i)++] = time_stamp;
			int temp = execute->executionTime;
			execute->executionTime -= t*(execute->priority);

			if(execute->executionTime<0)
				time_stamp += temp;
			else
				time_stamp += t*(execute->priority);
			
			execute->preempt_times[(execute->j)++] = time_stamp;

			if(execute->executionTime > 0) //still has to be inserted
			{
				execute->color = 1; //color should be red for any insertion process
				insert(root, execute);
			}	
			else
			{
				x--;
				execute->completion_time = time_stamp;
				print_data(execute);
			}

			scheduler(N, x, M, t, c, root);
		}
	}

}


void main()
{
	int N = 10, x = 0, M = 20; //max number of live processes, current number of processes, number of processes to be completed
	int t = 100; //time for each scheduling = priority*t;
	int c = 0; //total number of processes created

	srand(time(NULL));

	process* processTree = NULL;

	record = fopen("processes.txt", "w");
	fprintf(record, "Process ID  Process Priority  Creation Time  Execution Times\t\t\t\t\t\t\t\t\t\t\t\tPreempt Times\t\t\t\t\t\t\t\t\tCompletion Time");
	scheduler(N, x, M, t, c, &processTree);
	fclose(record);	
}

void insert5(process** root, process* n)
{
	process *g = grandparent(n);

	g->color = 1; //paint red
	n->parent->color = 0; //paint black
	
	if(n == n->parent->left)
		right_rotate(root, g);
	else
		left_rotate(root, g);		
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
