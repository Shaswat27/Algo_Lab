//Considering interval as both sides closed

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct li
{
	int k;
	struct li* next;
} list;

typedef struct i
{
	int a;
	int b;
	list* x;
	struct i* left;
	struct i* right;
} node;

node* CREATE_INTERVAL_TREE(int l, int u, int n)
{
	if(n == 0)
	{
		node* p = NULL;
		return p;
	}	

	int size = (u-l+1)/n;
	
	int median = l + size*( (n-1)/2 );
	
	node* root = (node *)malloc(sizeof(node));
	root->a = median;
	root->b = median + (size-1);
	root->x = NULL;
	root->left = NULL;
	root->right = NULL;
	
	if(n%2 == 1)
		root->left = CREATE_INTERVAL_TREE(l, median-1, n/2);
	else
		root->left = CREATE_INTERVAL_TREE(l, median-1, n/2-1);

	root->right = CREATE_INTERVAL_TREE((root->b+1), u, n/2);
		
	return root;	
}

void INSERT(node* T, int i)
{
	if(T==NULL) return;
		
	if( i>=(T->a) && i<=(T->b) ) //if it lies in this interval
	{
		if(T->x == NULL) //first list element
		{
			T->x = (list *)malloc(sizeof(list));
			T->x->k = i;
			T->x->next = NULL;
		}
		else
		{
			list* insert = (list *)malloc(sizeof(list));		
			insert->k = i;
			insert->next = NULL;	
			
			list* p = (T->x);
			
			if((p)->k>i) ///first element is greater
			{
				insert->next = p;	
				T->x = insert;
				return;
			}
			
			while( (p)->next != NULL ) 
			{
					if((p)->next->k > i)
						break;				
					(p)=(p)->next;
			}
			//p now points to the place of insertion
			insert->next = (p)->next;
			(p)->next = insert;			
		}
		return;
	}	
	else if( i<(T->a) )
		INSERT(T->left, i);
	else if( i>(T->b) )
		INSERT(T->right, i);	
}	


void list_delete(list* head, list* n)
{
	//When node to be deleted is head node
    if(head == n)
    {
        if(head->next == NULL)
        {
            return;
        }
 
        head->k = head->next->k;
 
        // store address of next node
        n = head->next;
 
        // Remove the link of next node
        head->next = head->next->next;
 
        // free memory
        free(n); 
        return;
    }
 
 
    //When not first node, follow the normal deletion process 
    //find the previous node
    list *prev = head;
    while(prev->next != NULL && prev->next != n)
        prev = prev->next;
 
    // Check if node really exists in Linked List
    if(prev->next == NULL)
    {
        return;
    }
    // Remove node from Linked List
    prev->next = prev->next->next;
    // Free memory
    free(n);
    return; 
}	

int flag = 0;
node** sub_root;
node* lower;
node* upper;
void MERGE(node* T, int l, int u)
{
	static int max_u = -32767;
	static int min_l = 32768;

	if(T == NULL) return;
	
	if( (T->a)>u ) //no intersection 
		MERGE(T->left, l, u);
	if( (T->b)<l ) //no intersection
		MERGE(T->right, l, u);
	
	//if we find an intersection
	if(!(l>=(T->b) || u<=(T->a)) )
	{
		if(!flag) //if we've found an intersection for the first time
		{
			sub_root = &T; 
			flag = 1;
			lower = (node *)malloc(sizeof(node));
			lower->a = 0; lower->b = 0; lower->x = NULL; lower->left = NULL; lower->right = NULL;
			upper = (node *)malloc(sizeof(node));
			upper->a = 0; upper->b = 0; upper->x = NULL; upper->left = NULL; upper->right = NULL;
		}

		//case 1 - T is completely completely inside [l,u]
		if( l<=(T->a) && (T->b)<=u)
		{
			if(*sub_root!=T) //if we are not at the subroot, insert all elemnts to 
			{
				list** head = &(T->x);
				list** p = &(T->x);
				while(*p!=NULL)
				{
					INSERT(*sub_root, (*p)->k);
					list_delete(*head, *p);
					*p = (*p)->next;
				}
			}
			MERGE(T->left, l, u);
			MERGE(T->right, l, u);
		}

		//case 2 - T's upper bound is inside
		else if( l<=(T->b) && l>=(T->a))
		{
			if(T->a < min_l) min_l = T->a;
			if(*sub_root!=T) //if we are not at the subroot, insert all elemnts to 
			{
				list** head = &(T->x);
				list** p = &(T->x);
				while(T->x!=NULL)
				{
					if((*p)->k>=l)
					{
						INSERT(*sub_root, (*p)->k);
						list_delete(*head, *p);
						*p = (*p)->next;
					}
					else
						INSERT(lower, (*p)->k);
				}					
			}
			MERGE(T->right, l, u);
		}

		//case 3 - T's lower bound is inside
		else if( u>=(T->a) && u<=(T->b))
		{
			if(T->b > max_u) max_u = T->b;
			if(*sub_root!=T) //if we are not at the subroot, insert all elemnts to 
			{
				list** head = &(T->x);
				list** p = &(T->x);
				while(T->x!=NULL)
				{
					if((*p)->k<=u)
					{
						INSERT(*sub_root, (*p)->k);
						list_delete(*head, *p);
						*p = (*p)->next;
					}
					else
						INSERT(upper, (*p)->k);
				}					
			}
			MERGE(T->left, l, u);
		}
		//case 4??
	}	
		
}	

node** first;
void PRETTY_PRINT(node* Tree)
{
	static int h = 0;
	static int rt = 0;

	if(Tree == NULL) {return;}

	if(!rt)
	{
		first = &Tree;
		rt = 1;
	}


	PRETTY_PRINT(Tree->left);

	int i=0;
	for(;i<h;i++)
		printf("\t");
	printf("[%d,%d] ", Tree->a, Tree->b);
	list* p = Tree->x;
	while(p != NULL)
	{
		if(p->next == NULL)
			printf("%d", p->k);
		else
			printf("%d, ", p->k);
		p = p->next;
	}
	printf("\n");
	h++;
	if(Tree == *first) h=0;

	PRETTY_PRINT(Tree->right);		
}		
					

int main()
{
	node* root;
	root = CREATE_INTERVAL_TREE(1,25,5);
	INSERT(root, 2);
	INSERT(root, 1);
	INSERT(root, 3);
	INSERT(root, 13);
	INSERT(root, 20);
	INSERT(root, 17);
	PRETTY_PRINT(root);
	return 0;
}
